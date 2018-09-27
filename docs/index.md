Sym is a set-driven matrix language for expressing large-scale general 
equilibrium models in algebraic form. It is designed to be nearly as 
concise as standard matrix notation. However, it also imposes rigorous 
conformability rules on all expressions, allowing it to catch a broad 
range of potential errors that could otherwise arise in matrix-driven 
languages. As a result, subscripts are unnecessary in many expressions, 
improving the clarity of a model's code and facilitating documentation 
and maintenance. 

Sym is used to implement G-Cubed, a multi-region general equilibrium model 
of the world economy, and [OpenIGEM](https://openigem.github.io/), 
an open-source econometric general equilibrium model of the US economy.

# Source Code and Executables
+ Sym's source code is available on GitHub via the link above or [here](https://github.com/pjwilcoxen/sym/).
+ Windows executables are available from the [releases page](https://github.com/pjwilcoxen/sym/releases/).
+ Sym also compiles without modification under various versions of Linux.

# Documentation
+ An [overview of Sym](sym-overview.pdf) and a short discussion of 
its design principles.
+ A [short example](comparing-notation.pdf) comparing Sym's 
notation with that of Fortran and Gempack's TABLO language.
+ An annotated listing of the [Sym implementation of the G-Cubed model](sym-gcubed-example.pdf).
+ The program's [usage message](usage.md), 
including a list of the back-end solution packages it supports. 
+ A brief [reference card](syntax.md) for Sym's syntax.

