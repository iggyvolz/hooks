/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 662ecf2ab981e74508d9025e15f87b2d7d9ea67b */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_iggyvolz_hooks_PreHook_before, 0, 3, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, method, IS_STRING, 0)
	ZEND_ARG_TYPE_MASK(0, target, MAY_BE_NULL|MAY_BE_OBJECT|MAY_BE_STRING, NULL)
	ZEND_ARG_TYPE_INFO(0, params, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_iggyvolz_hooks_PostHook_after, 0, 3, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, method, IS_STRING, 0)
	ZEND_ARG_TYPE_MASK(0, target, MAY_BE_NULL|MAY_BE_OBJECT|MAY_BE_STRING, NULL)
	ZEND_ARG_TYPE_INFO(0, retval, IS_MIXED, 0)
ZEND_END_ARG_INFO()




static const zend_function_entry class_iggyvolz_hooks_PreHook_methods[] = {
	ZEND_ABSTRACT_ME_WITH_FLAGS(iggyvolz_hooks_PreHook, before, arginfo_class_iggyvolz_hooks_PreHook_before, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT)
	ZEND_FE_END
};


static const zend_function_entry class_iggyvolz_hooks_PostHook_methods[] = {
	ZEND_ABSTRACT_ME_WITH_FLAGS(iggyvolz_hooks_PostHook, after, arginfo_class_iggyvolz_hooks_PostHook_after, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT)
	ZEND_FE_END
};

static zend_class_entry *register_class_iggyvolz_hooks_PreHook(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "iggyvolz\\hooks", "PreHook", class_iggyvolz_hooks_PreHook_methods);
	class_entry = zend_register_internal_interface(&ce);

	return class_entry;
}

static zend_class_entry *register_class_iggyvolz_hooks_PostHook(void)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "iggyvolz\\hooks", "PostHook", class_iggyvolz_hooks_PostHook_methods);
	class_entry = zend_register_internal_interface(&ce);

	return class_entry;
}
