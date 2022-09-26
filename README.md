# Zeropoint

**Zeropoint** (**Φ**) is an open C API specification to create, manage, transform and execute distributed dataflow graphs - giving software developers a design paradigm to build distributed software systems with minimal effort while being modular, flexible and composable.

**Zeropoint** supports a wide variety of applications, ranging from embedded software running on hardware-accelerated edge devices to large-scale, cloud based platforms through a unified low-level, high-performance, portable API abstraction. This API also seamlessly allows for composition of logic blocks (called **Nodes**) in a low/no-code environment. By creating this unified interface abstraction, Zeropoint hopes to form the foundational layer of an ecosystem of composable dataflow building blocks that non-technical users can build solutions from in low-code/no-code environments.

**Zeropoint** consists of:

* An API for creating **Nodes**, which are the smallest unit of composable work in this abstraction. **Nodes** are considered black-boxes that operate on the currently available input and produce outputs for the current time step.
* An API for creating transforms and applying them on **Nodes**, called **Portals**. **Portals** allow transformation of any **Nodes** acquired through them, allowing us to create local or distributed graphs, render views of **Nodes** via a specific **Portal** and create any number of abstractions that operate on these graphs to produce other graphs via a transformation.
* An API to coordinate data transfer between **Nodes** that is pipelining friendly, using **Queues** and **Events** to fence reads and writes across all parts of the (distributed) dataflow.

It is a native, cross-platform C library that acts as an installable runtime to detect, load and appropriately call **Zeropoint** **Modules** that expose an implementation of this API as a set of C-callable functions to create and manipulate **Portals**, **Nodes**, **Queues** and **Events** in any other language, runtime or environment.

The API is further divided into a core specification that any **Zeropoint** compliant **Module** must support as well as optional extensions that can be moved into the core specification as needed. This allows vendors to create and test changes to both the core specification and custom **Zeropoint Modules** that require it.
