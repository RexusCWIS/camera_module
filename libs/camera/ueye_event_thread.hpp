/**
 * @file ueye_event_thread.hpp
 * @brief uEye camera event management thread class definition.
 */

#ifndef DEF_UEYE_EVENT_THREAD_HPP
#define DEF_UEYE_EVENT_THREAD_HPP

#include "ueye_camera.hpp"
#include <uEye.h>
#include <pthread.h>

class UEye_Camera; 

/**
 * @brief uEye camera event management thread class.
 * @details This class represents a thread dedicated to the handling of a specific camera event
 *          (acquisition, status, hardware failure...). It spawns a thread that handles the event
 *          and calls a callback function for that specific event. 
 */
class UEye_EventThread {

    public:
        /**
         * @brief Constructor. Sets the camera ID, the event type and the callback function. 
         */
        UEye_EventThread(const UEye_Camera *camera, int event, void (*callback)(const UEye_Camera *)); 

        /**
         * @brief Detructor. Stops the event handling and disables the event. 
         */
        ~UEye_EventThread(); 
 
        /**
         * @brief Returns the status of the event thread. 
         * @returns True if the event thread is running. 
         */
        bool isRunning(void) const; 

        /**
         * @brief Starts the event listening.
         */
        void start(void); 

        /** 
         * @brief Stops the event listening. 
         */
        void stop(void);

    private:
        const UEye_Camera* m_camera;                    /**< Pointer to the camera object whose event is managed by this instance. */ 
        int  m_event;                                   /**< ID of the event handled by this instance. */
        void (*m_eventCallback)(const UEye_Camera *);   /**< Callback function to handle the event. */
        pthread_t m_eventThread;                        /**< Thread structure. */
        bool m_stop;                                    /**< Order to stop the event listening. */
        bool m_running;                                 /**< Event listening status. */

        /**
         * @brief Waits for the handler thread to end.
         */
        void waitForThreadEnd(void); 

        /**
         * @brief Event handler thread. 
         * @details This function represents the event thread. It waits for the given event and performs 
         *          the callback to the handler function passed when the instance of the class was constructed. 
         */
        static void * handler(void *arg);
};

#endif  /* DEF_UEYE_EVENT_THREAD_HPP */

