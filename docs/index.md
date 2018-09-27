# Abstract

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

# Documentation

+ [Overview](sym-overview.pdf): an overview of Sym and a short discussion of 
its design principles.

+ [Notation](comparing-notation.pdf): a short example comparing Sym's 
notation with that of Fortran and Gempack's TABLO language.

+ [G-Cubed](sym-gcubed-example.pdf): an annotated example of the use
of Sym in implementing the G-Cubed model.

+ [Usage](usage.md): a short usage description for the program, 
including a list of the back-end solution packages that are currently 
supported. 

+ [Syntax](syntax.md): a brief overview of Sym's syntax.
