#ifndef DYNAMICCONFIGITEMDATASTRUCT_H_ 
#define DYNAMICCONFIGITEMDATASTRUCT_H_

#include <iostream>

const int max_item_value_size_c = 256;
const int max_item_name_size_c = 41;
const int max_item_value_type_size_c = 13;

struct DynamicConfigItemDataStruct{
        char conf_item_value[max_item_value_size_c];
        char conf_item_name[max_item_name_size_c];
        char conf_item_value_type[max_item_value_type_size_c];

};

#endif /* DYNAMICCONFIGITEMDATASTRUCT_H_ */

