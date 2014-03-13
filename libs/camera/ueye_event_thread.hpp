/**
 * @file ueye_event_thread.hpp
 * @brief uEye camera event management thread class definition.
 */

#ifndef DEF_UEYE_EVENT_THREAD_HPP
#define DEF_UEYE_EVENT_THREAD_HPP

#include "ueye_camera.hpp"
#include <uEye.h>
#include <pthread.h>

class ueye_camera;

/**
 * @brief uEye camera event management thread class.
 * @details This class represents a thread dedicated to the handling of a specific camera event
 *          (acquisition, status, hardware failure...). It spawns a thread that handles the event
 *          and calls a callback function for that specific event. 
 */
class ueye_event_thread {

    public:
        /**
         * @brief Constructor. Sets the camera ID, the event type and the callback function. 
         */
        ueye_event_thread(const ueye_camera *camera, int event, void (*callback)(const ueye_camera*)); 

        /**
         * @brief Detructor. Stops the event handling and disables the event. 
         */
        ~ueye_event_thread(); 
 
        /**
         * @brief Returns the status of the event thread. 
         * @returns True if the event thread is running. 
         */
        bool is_running(void) const; 

        /**
         * @brief Starts the event listening.
         */
        void start(void); 

        /** 
         * @brief Stops the event listening. 
         */
        void stop(void);

    private:
        const ueye_camera* m_camera;                    /**< Pointer to the camera object whose event is managed by this instance. */ 
        int  m_event;                                   /**< ID of the event handled by this instance. */
        void (*m_event_callback)(const ueye_camera*);   /**< Callback function to handle the event. */
        pthread_t m_event_thread;                       /**< Thread structure. */
        bool m_stop;                                    /**< Order to stop the event listening. */
        bool m_running;                                 /**< Event listening status. */

        /**
         * @brief Waits for the handler thread to end.
         */
        void wait_for_thread_end(void); 

        /**
         * @brief Event handler thread. 
         * @details This function represents the event thread. It waits for the given event and performs 
         *          the callback to the handler function passed when the instance of the class was constructed. 
         */
        static void * handler(void *arg);
};

#endif  /* DEF_UEYE_EVENT_THREAD_HPP */

