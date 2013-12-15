#include "stdafx.h"

/* declaration of functions to be exported */
ZEND_FUNCTION(DoubleUp);

/* compiled function list so Zend knows what's in this module */
zend_function_entry CustomExtModule_functions[] = {
    ZEND_FE(DoubleUp, NULL)
    {NULL, NULL, NULL}
};

/* compiled module information */
zend_module_entry CustomExtModule_module_entry = {
    STANDARD_MODULE_HEADER,
    "CustomExt Module",
    CustomExtModule_functions,
    NULL, NULL, NULL, NULL, NULL,
    NO_VERSION_YET, STANDARD_MODULE_PROPERTIES
};

/* implement standard "stub" routine to introduce ourselves to Zend */
ZEND_GET_MODULE(CustomExtModule)

/* DoubleUp function */
/* This method takes 1 parameter, a long value, returns
   the value multiplied by 2 */
ZEND_FUNCTION(DoubleUp){
	long paramValue = 0;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &paramValue) == FAILURE) {
        RETURN_STRING("Bad parameters!", true);
    }
	paramValue *= 2;
	RETURN_LONG(paramValue);
}
