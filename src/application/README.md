

# Application





## Namespace

All functions of application is under the `rh` name space. For example,

`rh::Application` | `rh::AppThread` | ...





## Task Distribution

| Task ID         | Type    | Minimum Memory | Period | Description                 |
| --------------- | ------- | -------------- | ------ | --------------------------- |
| `__task_747842` | Dynamic | 256KB          | 1 TICK | Clock wheel ui angle update |
| `__task_264978` | Dynamic | 256KB          | N/A    | Load the screen first time  |
| `__task_047685` | Dynamic | 1024KB         | N/A    | Device initialization       |

See more details at `rh_thread.cc`.





## Shared Resource

The `rh::AppResource` contains global time information for threads to read. The time information will automatically updated inside the user timer interrupt function.



See more details at `rh_resource.cc`

