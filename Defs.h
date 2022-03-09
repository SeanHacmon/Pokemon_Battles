//
// Created by Sean Hakmon on 10/12/2021.
//

#ifndef HW3_DEFS_H
#define HW3_DEFS_H
typedef void* element;

typedef enum e_bool { false,true } bool;
typedef enum e_status {success, failure } status;
typedef enum ADT_status {DeleteSuccess, AddSuccess, DeleteFailure, AddFailure} GenStatus;
typedef status (*freeFunction) (element);
typedef element (*copyFunction)(element);
typedef int (*equalFunction) (element, element);
typedef status (*printFunction)(element);
typedef char* (*getCategoryFunction)(element);
typedef int (*getAttackFunction)(element ele1, element ele2, int* Atk1, int* Atk2);




#endif //HW3_DEFS_H


