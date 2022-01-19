# smart-watch

  
## Observer Pattern
The sensor class forms a concrete Subject that stores a reference to the Observer which in our case is the Notification class. 

As soon as there's a sensor data ready to be pushed to a notification list, it invokes a corresponding `Subject::notify()` that in turn calls a correspondg update() call of Notification classes that takes care of updating the notification list with a desired data.

This pattern is used for scalability: for X amount of sensors to be used (in the future), each of which could be attached by the `Notification` class. And there's always flexibility to unsubscribe any sensor from the Notification class.

<img width="1008" alt="Screen Shot 2022-01-18 at 4 23 51 PM" src="https://user-images.githubusercontent.com/15020079/150040221-4bba6b52-3282-4a55-ad7a-98a3c334a869.png">
