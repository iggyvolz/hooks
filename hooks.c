// include the PHP API itself
#include <php.h>
#include "Zend/zend_observer.h"
#include "zend_attributes.h"

zend_llist *hook_callback_functions = 0;
zif_handler* hookFunction;
zend_execute_data *_execute_data;
static void observer(zend_execute_data *execute_data, zval *return_value)
{
    bool begin = (return_value == NULL);
    zval hookName;
    // TODO register PreHook and PostHook internally
    ZVAL_STRINGL(&hookName, estrdup(begin ? "PreHook" : "PostHook"), strlen(begin ? "PreHook" : "PostHook"));
    zval hookMethodName;
    // TODO register PreHook and PostHook internally
    ZVAL_STRINGL(&hookMethodName, estrdup(begin ? "before" : "after"), strlen(begin ? "before" : "after"));
    zend_class_entry * hookInterface = zend_lookup_class(Z_STR(hookName));
    if(!hookInterface) return;
    // Loop through every attribute to see if we should call it
    zval* attribute_zval;
    zend_hash_internal_pointer_reset(execute_data->func->common.attributes);
    while(attribute_zval = zend_hash_get_current_data(execute_data->func->common.attributes)) {
        zend_attribute *attribute = (zend_attribute *)(attribute_zval->value.ptr);
        // Attempt to look up the class
        zend_class_entry * ce = zend_lookup_class(attribute->name);
        if(!ce)
        {
            zend_hash_move_forward(execute_data->func->common.attributes);
            continue;
        }
        if(instanceof_function(ce, hookInterface))
        {
            // Call desired function
            zend_object* object = zend_objects_new(ce); // TODO call constructor here
            hookFunction = (zif_handler*)(zend_hash_find(&(ce->function_table), hookMethodName.value.str)->value.ptr);
            zval retval;
            zend_fcall_info fci;
            fci.function_name = hookMethodName;
            fci.named_params = NULL;
            fci.param_count = begin ? 2 : 3;
            zval params[3];
            // execute_data->func->common.function_name->val;
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
            if(!begin) {
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
    "0.1.0",
    STANDARD_MODULE_PROPERTIES
};

// use a macro to output additional C code, to make ext dynamically loadable
ZEND_GET_MODULE(hooks)
