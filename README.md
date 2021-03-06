# Sym

Sym is a set-driven matrix language for expressing large-scale general 
equilibrium models in algebraic form. It is designed to be nearly as 
concise as standard matrix notation. However, it also imposes rigorous 
conformability rules on all expressions, allowing it to catch a broad 
range of potential errors that could otherwise arise in matrix-driven 
languages. As a result, subscripts are unnecessary in many expressions, 
improving the clarity of a model's code and facilitating documentation 
and maintenance. 

This site provides code for a program, also called sym, that reads
and analyzes models written in Sym and then writes them out in forms
suitable for a range of solution algorithms. In essence, it provides a 
Sym front end, with concise notation and extensive conformability rules, 
for other software. Support for back-end solution packages is modular 
and straightforward to extend to other others.

Sym is used to implement G-Cubed, a multi-region general equilibrium model 
of the world economy, and [OpenIGEM](https://openigem.github.io/), 
an open-source econometric general equilibrium model of the US economy.

For more information and documentation, please see 
the [sym documentation site](https://pjwilcoxen.github.io/sym/).
To download a Windows binary of sym, please see the 
[releases page](https://github.com/pjwilcoxen/sym/releases).
