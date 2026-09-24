Everything is snake_case apart from the name of the engine which is Module (the root folder).
classes, filenames, namespaces. This enforces good naming to differentiate between functions, classes and also lowers the barrier or reading stuff. For example you should still use m for member variables, etc.

No camel case
no one line if statements


includes:
<> for external of module like SDK or libraries
"" for internal headers / close

try to match one header and one source for one class the one that's named after the file so avoid something like: core.hpp with a bunch of shit. do something like window_module.hpp and have window_module class there

also for include try to keep the cpp includes ordered and if you need anything include it through its source include

first put the external includes

second put the internal includes

pragma once in header
