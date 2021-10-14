Transitioning to C
==================

Henceforth, we'll be using the C programming language instead of C0.  C is one
of the most widely used languages for writing system software for large and
small (i.e. "embedded") systems. Examples include the Linux kernel itself which
has over 25million lines of C code today. The language specification that we'll
be using for this course is called `C99 <https://en.wikipedia.org/wiki/C99>`_.
The reason for this popularity is several fold -

1. C gives you very low level control of system resources like memory, which is
   important in constrained systems. The early systems for which the C language
   itself was developed would look extremely constrained from today's perspective.

2. With the long history that C has (it was born in 1972), C compilers have gotten
   extremely good at producing highly optimized code, which again is beneficial
   to all systems large and small.

The downside of the language is that its strength of giving the programmer a
lot of control over system resources can itself be a problem when developing
application software, since the author is forced to deal with these resources
in every circumstance. Derived languages like "C++" and "Objective-C" add
features that make routine record keeping of resources much more manageable and
less error prone.

That said, it is a great language to get to grips with the machine model that
we all use today - the model where a processor runs a loop of the form -

1. (implicit) Fetch instruction - which can tell the processor to do one of ...
2. Fetch data from memory into registers
3. Compute result
4. Store result from registers into memory

The above is a simplified picture and modern "super scalar" processors are far
more complicated, but that deserves a whole course on its own and we'll live
with the above sketch for the purpose of this course.


The C memory model
------------------

When we began this course, we looked at the "spreadsheet" model of memory and
how we can trace and understand the state of a program. That's not far from the
truth. The computer's addressable memory is like an array of single bytes whose
indices serve as their "address". The "RAM" -- random access memory -- when
given an address, can give us the byte stored at that address or store a given
byte at an address. On systems today, our programs get such a virtual array
that's about 4GB in size on 32-bit machines and 16billion GB in size on 64-bit
machines.  Of course we don't have that much physical memory, so the kernel
uses storage like hard disks or SSDs to extend the memory available to our
programs without us having to think about it. It is **as though** the RAM is
modeled by the following functions ::

    uint8_t ram_load(uint64_t address);
    void ram_store(uint64_t address, uint8_t byte);

Since the round trip to the RAM and back taken time relative to how fast our
processors can compute today, this interface is extended to fetch blocks of
memory at a time, so in reality it is more like ::

    block_t ram_load_block(uint64_t address);
    void ram_store(uint64_t address, block_t b);

where we've used the name ``block_t`` to stand for the block. (i.e. it is not a
real data type in C.)

The C stack
-----------

In addition to the main memory bulk, the C language also provides a "stack"
that is tied to scoped variables in the language.

.. NOTE:: The scope of a variable in C is determined by the inner most pair
   of braces ``{}`` that enclose the definition of the variable.

Consider the following example code -

.. code-block:: C

    int accumulate(int N, int *in, int *out) {
        int val = 0;
        int i = 0;
        for (i = 0; i < N; i++) {
            int tmp = val + in[i];
            out[i] = tmp;
            val = tmp;
        }
        return val;
    }

Your program will need some memory to store the variables ``val`` (4-bytes),
``i`` (4-bytes) and ``tmp`` (4-bytes) and also for the arguments to your
function ``N`` (4-bytes), ``in`` (8-bytes) and ``out`` (8-bytes). Since ``in``
and ``out`` are pointer types, they usually are stored as 64-bit address values.

The storage for these variables is allocated and managed by the compiler
automatically using a "stack" structure. A variable is given some memory
on the stack only while the code within its immediate scope is executing.
So a variable is said to be "visible" only within its scope. Once a scope
is exited, the compiler may release the storage needed for the variable.

In the above example, the ``tmp`` variable is local to the ``for`` loop
and is not available at the point where we're doing ``return val;``.
However, the variables ``val``, ``i`` and so on are available within
the ``for`` loop as its scope is contained in the scope of those variables.

Why is this called a "stack"? You can think of an open brace ``{`` as an
instruction to "push new variables on to the stack" and the matching ``}`` as
an instruction to pop all the variables up to the matching ``{`` from the
stack. So the stack gets deeper the more nested scopes there are (including
function calls which add their own scope) and when these scopes are exited, the
stack becomes shallower again.  This is why if your function is recursive and
makes too many recursive calls, you may run out of stack space and your program
may crash with a "stack overflow" error.

The stack is not very visible in C0 since you can only work with it via the
scopes. In C though, you can take the address of variables allocated on the
stack and pass them around .. while making sure you don't use them after the
variable goes out of scope .. which means the stack is very much visible
to the programmer and the program.


Differences from C0
-------------------

For the most part, C0 is a subset of C, so if you write C0 code for your
functions, you'll be writing valid C code. The difference will be in the
functions available to you in the two languages. C offers more to the
adventurous though. Below we consider the language differences from several
perspectives.

.. list-table:: C0 versus C - pragmatics
   :widths: 20,40,40
   :header-rows: 1

   * - **Task**
     - **For C0**
     - **For C**
   * - Compile a program
     - ``cc0 program.c0``
     - ``gcc program.c`` or ``tcc program.c`` or ``clang program.c``
   * - Run compiled program
     - ``./a.out``
     - ``./a.out``
   * - Compile with contract checks/debugging enabled.
     - ``cc0 -d program.c0``
     - ``gcc -d program.c`` (same for others)
   * - Compile with optimizations
     - Nothing particular
     - ``gcc -O3 program.c`` - turns on maximum "level 3" optimizations.
   * - Use standard libraries
     - ``#use <conio.h>`` etc. Must occur first in source file.
     - Include the library's headers. Common ones given below ::

           #include <stdio.h>
           #include <stdlib.h>
           #include <math.h>
           #include <assert.h>

       They can technically occur anywhere as ``#include <stdio.h>`` just
       means "copy paste contents of stdio.h into my code at this point",
       however they're usually placed at top of source file.
   * - Contracts
     - ``@requires``, ``@ensures`` and ``@assert``
     - Not supported in general, but if you ``#include <assert.h>``,
       an ``assert`` "macro" becomes available which can be used to
       declare known properties at certain points in your code ::

            assert(some_variable == some_value);

       Any boolean expression may be used as the argument to ``assert``.
       When compiling using ``gcc -O3``, all ``assert`` expressions are
       slided from the code.

.. list-table:: C0 versus C - basic language features
   :widths: 20,40,40
   :header-rows: 1

   * - **Feature**
     - **In C0**
     - **In C**
   * - Statement structure
     - ``<statement>;``
     - Same - i.e. terminated by a semi-colon.
   * - Scope delimiters
     - A pair of braces -- i.e. ``{<code>}``
     - Same
   * - Number types
     - Only 32-bit signed integers - ``int``
     - Many sized integers both signed and unsigned.
       Ex: ``int`` (signed "word"-sized integer) ::

           int8_t / uint8_t 
           int16_t / uint16_t
           int32_t / uint32_t
           int64_t / uint64_t
   * - Floating point numbers
     - Not supported
     - ``float`` (32-bit) and ``double`` (64-bit)
       types are available.
   * - Character type
     - ``char``
     - ``char``
   * - Boolean type
     - ``bool``
     - You need to ``#include <stdbool.h>`` and then
       you can use ``bool`` along with ``true`` and ``false``.
   * - Get address of a variable
     - Not allowed. You need can only get pointers using ``alloc``.
     - If ``x`` is a variable of type ``T``, then ``&x``
       gives its address and is an expression of type ``T*`` --
       i.e. a pointer to ``T``. ``&`` can also be used on
       members of ``struct`` ::

            struct Pair {
                int x;
                int y;
            };

            struct Pair p1;
            p1.x = 42;
            p1.y = 23;

            int *yptr = &p1.y;
            printf("%d\n", *yptr); // Prints 23


   * - Get contents of a pointer
     - If ``p`` is of type ``T*``, then its contents are obtained
       using ``*p``.
     - Same as C0.
   * - Treat boolean as integer
     - Not allowed
     - ``(int)b`` where ``b`` is declared as ``bool b;``
       A "boolean" value in C is equivalent to an integer
       that is 0 to represent falsehood and 1 to represent
       truth. So ``printf("%d\n", b)`` will print either
       0 or 1 where ``b`` is a ``bool`` value.
   * - String type
     - Native type ``string``
     - Modeled as "pointer to character" (``char*``) with
       the end of string signified by a character with numeric code ``0``
       called the "null character", notated as ``'\0'``. When the C
       compiler sees a string constant notated as ``"hello"``,
       it automatically inserts the null character at the end,
       so the storage used for ``"hello"`` is actually 6 bytes
       and not 5 bytes.
   * - Array type
     - Native type ``T []`` allocated using ``alloc_array(T, n)``.
     - An array of ``T`` is essentially a pointer at which ``n``
       consecutive values of type ``T`` are stored. If ``T *p``,
       then you can access the ``T`` at index 4 using ``p[4]``
       or equivalently ``*(p+4)``. C also has an array syntax
       ``int arr[10];`` that can be used for temporary "stack" 
       allocation and for sequences stored in ``struct``s. Such
       a variable ``arr`` is effectively a pointer though.
       This adds more confusion at this stage, so we'll not be 
       using C style arrays and will stick to pointer-based approach.
   * - Print things
     - Use ``printint`` or ``print`` as appropriate.
     - ``#include <stdio.h>`` provides "formatted print"
       ``printf(<format-string>, <additional-values>)``.
       So for example ``printf("%d+%d=%d\n", 2, 3, 5);``
       will print "2+3=5". See documentation of ``printf``
       for more details about format codes. ``printf`` will
       automatically flush the output when it prints a new-line
       character ``'\n'``.
   * - Define type names
     - Ex: ``typedef struct Node *node_t;``
     - Same

.. list-table:: C0 versus C - memory management
   :widths: 20,40,40
   :header-rows: 1

   * - **Task**
     - **In C0**
     - **In C**
   * - Allocate memory for a type ``T``
     - ``T *x = alloc(T);``
     - ``T *x = (T*)malloc(sizeof(T));``
   * - Allocate memory for an array of ``N`` elements of type ``T``.
     - ``T []x = alloc_array(T, N);``
     - ``T *x = (T*)malloc(N * sizeof(T));``
   * - Free allocated memory.
     - Not needed since garbage collector takes care of it.
     - ``free(x)`` where x stores a pointer returned by ``malloc``.
       Not freeing allocated memory in C results in a "memory leak".
   * - Casting
     - Restricted scenarios in C1 language extension
     - 1. Free casting between pointer types using ``(T2*)v`` where
          ``v`` is of type ``T1*``.
       2. Free casting between structure types is an error.
       3. Free casting between ``int`` and ``bool`` or ``char``
          since all three are integers under the hood. The compiler
          may warn when you cast a "larger type" to a "smaller type" --
          ex: from ``int`` to ``bool``.
   * - Treat character as integer
     - Use ``int char_ord(char ch)`` function in ``string`` library.
     - ``(int)ch`` where ``ch`` is a ``char``.
   * - Treat integer as character
     - Use ``char char_chr(int i)`` function in ``string`` library.
     - ``(char)i`` where ``i`` is an integer type.

.. list-table:: C0 versus C - iteration
   :widths: 20,40,40
   :header-rows: 1

   * - **Feature**
     - **In C0**
     - **In C**
   * - For loops
     - ``for (<init>;<cond>;<step>) { <body> }``
     - Same
   * - While loops
     - ``while (<cond>) { <body> }``
     - Same
   * - Break out of ``for`` and ``while`` loops
     - Not supported
     - ``break;`` statement
   * - Jump back to loop condition ignoring rest of loop body.
     - Not supported
     - ``continue;`` statement
   * - Returning from a function
     - ``return <expr>;``
     - Same
   * - Functions
     - ``<return-type> <fn-name>(<arg1-type> <var1>, ...) { <body> }``
     - Same


