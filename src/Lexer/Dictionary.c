#include "Dictionary.h"

// TODO : dans le futur il y aura surement des versions françaises de ces tableaux afin de supporter la version française du pseudocode

//(les indices sont spécifiés explicitements au cas ou des changements sont effectué à l'enum des Keywords dans le futur)
const char *c_keywords_strings_en[] = {
        [Keyword_Algorithm] = "algorithm",
        [Keyword_Function]  = "function",
        [Keyword_Constants] = "constants",
        [Keyword_Variables] = "variables",
        [Keyword_Of]        = "of",
        [Keyword_Begin]     = "begin",
        [Keyword_End]       = "end",
        [Keyword_If]        = "if",
        [Keyword_Then]      = "then",
        [Keyword_Else]      = "else",
        [Keyword_While]     = "while",
        [Keyword_For]       = "for",
        [Keyword_To]        = "to",
};
const size_t c_keywords_strings_en_size = sizeof(c_keywords_strings_en) / sizeof(const char *);

//(Ici également)
const char *c_vartypes_strings_en[] = {
        [VariableType_Bool]         = "boolean",
        [VariableType_Int]          = "integer",
        [VariableType_Float]        = "float",
        [VariableType_Character]    = "character",
        [VariableType_String]       = "string",
        [VariableType_Array]        = "array"
};
const size_t c_vartypes_strings_en_size = sizeof(c_vartypes_strings_en) / sizeof(const char *);

//(Ici également)
const char *c_operators_strings_en[] = {
        [Operator_Assignment]       = "<-",
        [Operator_Plus]             = "+",
        [Operator_Minus]            = "-",
        [Operator_Multiplication]   = "*",
        [Operator_Division]         = "/",
        [Operator_Equal]            = "=",
        [Operator_Inferior]         = "<",
        [Operator_InferiorEqual]    = "<=",
        [Operator_Superior]         = ">",
        [Operator_SuperiorEqual]    = ">=",
        [Operator_RightByteShift]   = ">>",
        [Operator_LeftByteShift]    = "<<",
        [Operator_Not]              = "not",
        [Operator_And]              = "and",
        [Operator_Or]               = "or"
};
const size_t c_operators_strings_en_size = sizeof(c_operators_strings_en) / sizeof(const char *);
