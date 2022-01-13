// include the PHP API itself
#include <php.h>
#include "Zend/zend_observer.h"
#include "zend_attributes.h"
#include "hooks_arginfo.h"

zend_class_entry *preHook, *postHook;

static void observer(zend_execute_data *execute_data, zval *return_value)
{
    bool begin = (return_value == NULL);
    zend_class_entry *hookInterface = begin ? preHook : postHook;
    if(!hookInterface) return;
    // Loop through every attribute to see if we should call it
    zval* attribute_zval;
    zend_hash_internal_pointer_reset(execute_data->func->common.attributes);
    while((attribute_zval = zend_hash_get_current_data(execute_data->func->common.attributes))) {
        zend_attribute *attribute = (zend_attribute *)(attribute_zval->value.ptr);
        // Attempt to look up the class
        zend_class_entry *ce = zend_lookup_class(attribute->name);
        if(!ce)
        {
            zend_hash_move_forward(execute_data->func->common.attributes);
            continue;
        }
        if(instanceof_function(ce, hookInterface))
        {
            // Call desired function
            zend_object* object = zend_objects_new(ce); // TODO call constructor here
            zval retval;
            zend_fcall_info fci;
            ZVAL_STRINGL(&fci.function_name, estrdup(begin ? "before" : "after"), strlen(begin ? "before" : "after"));
            fci.named_params = NULL;
            fci.param_count = 3;
            zval params[3];
            ZVAL_STRINGL(&params[0], estrdup(execute_data->func->common.function_name->val), strlen(execute_data->func->common.function_name->val));
            params[1] = execute_data->This;
            if(Z_TYPE(params[1]) == IS_UNDEF) {
                // This is not an object call
                if(execute_data->func->common.scope) {
                    // This is a static call
                    ZVAL_STRINGL(&params[1], estrdup(execute_data->func->common.scope->name->val), strlen(execute_data->func->common.scope->name->val));
                } else {
                    // This is a plain method call
                    ZVAL_NULL(&params[1]);
                }
            }
            if(begin) {
                array_init(&params[2]);
                int arg_count = ZEND_CALL_NUM_ARGS(execute_data);
                for (int i = 1; i <= arg_count; i++)
                {
                    add_next_index_zval(&params[2], ZEND_CALL_ARG(execute_data, i));
                }
            } else {
                params[2] = *return_value;
            }
            fci.params = params;
            fci.object = object;
            fci.retval = &retval;
            zend_call_function(&fci, NULL);
        }
        zend_hash_move_forward(execute_data->func->common.attributes);
    }
    return;
}

static void observer_begin(zend_execute_data *execute_data) {
    observer(execute_data, NULL);
}

static void observer_end(zend_execute_data *execute_data, zval *return_value) {
    observer(execute_data, return_value);
}

static zend_observer_fcall_handlers global_hook(zend_execute_data *execute_data) {
	zend_observer_fcall_handlers handlers = {NULL, NULL};
	if (!execute_data->func ||
		!execute_data->func->common.function_name ||
        !execute_data->func->common.attributes) {
		return handlers; // I have no handlers for this function
	}
    
	handlers.begin = observer_begin;
	handlers.end = observer_end;
	return handlers; // I have handlers for this function
}

static PHP_MINIT_FUNCTION(observer)
{
	zend_observer_fcall_register(global_hook);
    preHook = register_class_iggyvolz_hooks_PreHook();
    postHook = register_class_iggyvolz_hooks_PostHook();
	return SUCCESS;
}

// some pieces of information about our module
zend_module_entry hooks_module_entry = {
    STANDARD_MODULE_HEADER,
    "hooks",
    NULL,
    PHP_MINIT(observer),
    NULL,
    NULL,
    NULL,
    NULL,
    "0.2.0",
    STANDARD_MODULE_PROPERTIES
};

// use a macro to output additional C code, to make ext dynamically loadable
ZEND_GET_MODULE(hooks)
