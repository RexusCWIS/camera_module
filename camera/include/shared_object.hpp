/**
 * @file shared_object.hpp
 * @brief Represents an object shared between multiple threads and protected by a mutex. 
 */

#ifndef DEF_SHARED_OBJECT_HPP
#define DEF_SHARED_OBJECT_HPP

#include <pthread.h>

template <typename T>
class SharedObject {    
    public: 
        SharedObject() {
            pthread_mutex_init(&mutex, NULL);
        }

        ~SharedObject() {
            pthread_mutex_destroy(&mutex); 
        }
        
        void lock(void) {
            pthread_mutex_lock(&mutex);     
        }
        void unlock(void) {
            pthread_mutex_unlock(&mutex); 
        }
        
    private: 
        T m_object; 
        pthread_mutex_t m_mutex; 
};

#endif  /* DEF_SHARED_OBJECT_HPP */

