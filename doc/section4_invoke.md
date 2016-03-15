Invoke
======

Invoke is one pretty cool feature. It calls a function that receives services with the right set of parameters.

Let's say we have this function:

    int doThings(Notification n, FileManager& fm);
    
Then you can use the container to call the function:

    int result = container.invoke<NotificationService, FileManagerService>(doThings);

This is good, it will work but we can make it better.

The `invoke` function can found what parameters the function needs.
But we need a way to associate parameters to services. Introducing the service map:

    template<typename>
    struct ServiceMap;
    
The service map is a template class that has the parameter type as template parameter and is using the `Service` type, which equal to which service the ;arameter should be associated.

For each service in which you want to be able work with `invoke` without explicitely listing every services, specialize the struct like this:

    template<> struct ServiceMap<Notification> : kgr::Map<NotificationService> {};
    template<> struct ServiceMap<FileManager&> : kgr::Map<FileManagerService> {};
    
Alternatively, you can make your ServiceMap using `Service` as the service it maps:

    template<> struct ServiceMap<Notification> { using Service = NotificationService; };
    template<> struct ServiceMap<FileManager&> { using Service = FileManagerService;  };

> Note the presence of the `&` after `FileManager`. This is because `FileManager`, `FileManager&` and `const FileManager*` could all be bound to different service definitions.
> Take note that you must declare the service map yourself.

Now that our service map is defined, we can use invoke like this:

    int doThings(Notification n, FileManager& fm);
    
    // ...
    
    int result = container.invoke<ServiceMap>(doThings);
    
With `ServiceMap`, the container can even call function from third party libraries!
 
[Next chapter](section5_setters.md)