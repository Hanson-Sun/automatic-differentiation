# automatic-differentiation
 
A collection of AD algorithms implemented in C++, with some basic performance benchmarks.

(maybe ill cook python bindings if i have time)

Only support functionals (functions of the form $f: \mathbb{R}^n \to \mathbb{R}$) for now.


## Dual Numbers
We define dual numbers as $\mathbb{R}[\varepsilon] = \{a + b \varepsilon | a, b \in \mathbb{R}\}$ where $\varepsilon^2 = 0$. We can define addition and multiplication as follows:
$$(a + b \varepsilon) + (c + d \varepsilon) = (a + c) + (b + d) \varepsilon$$
$$(a + b \varepsilon) \cdot (c + d \varepsilon) = ac + (ad + bc) \varepsilon$$

To find the what a function $f(x)$ evaluates to at $x + \varepsilon y$, by considering the Taylor expansion of $f(x + \varepsilon y)$.
$$f(x + \varepsilon y) = f(x) + f'(x) \varepsilon y + \frac{f''(x)}{2} \varepsilon^2 y^2 + \ldots$$
$$= f(x) + yf'(x) \varepsilon $$


So heres the magic part: we can get the derivative by just looking at the dual part of the evaluation.

Ok here is why it works. Consider an arbitrary function $f(x)$ written as a composition of elementary functions $f(x) = f_n(f_{n-1}(\ldots f_1(x)))$. Consider the evaluation of $f(x + \varepsilon y)$:
$$f(x + \varepsilon y) = f_n(f_{n-1}(\ldots f_1(x + \varepsilon y)))$$
$$= f_n(f_{n-1}(\ldots f_1(x) + \varepsilon f_1'(x) y))$$
$$= f_n(f_{n-1}(\ldots f_1(x))) + yf_n'(f_{n-1}(\ldots f_1(x))) f_1'(x) \varepsilon$$

Notice if $y = 1$, the dual part of the evaluation is the same as the chain rule

$$f'(x) = f_n'(f_{n-1}(\ldots f_1(x))) f_{n-1}'(\ldots f_1(x)) \ldots f_1'(x)$$




## Forward Mode AD

This is very similar to how dual numbers work, but instead we apply the "expansion" algorithmically by traversing the computational graph of the function. Consider the chain rule of an arbitrary function $f(x)$:
$$f'(x) = f_n'(f_{n-1}(\ldots f_1(x))) f_{n-1}'(\ldots f_1(x)) \ldots f_1'(x)$$
Now we basically want to evaluate from right to left
1. get $x_1 = f(x)$ and $f'(x) \cdot 1$
2. get $x_2 = f_1(x_1)$ and $f_1'(x_1) \cdot f'(x) \dots$
3. get $x_3 = f_2(x_2)$ and $f_2'(x_2) \cdot f_1'(x_1) \dots$
4. $\ldots$
5. get $x_n = f_{n-1}(x_{n-1})$ and $ f_{n-1}'(x_{n-1})$

Err so then the final derivative would just be $x_n$.

**we start from the input**


## Reverse Mode AD

This requires a forward pass and then a backwards pass. This actually allows to "accumulate" the derivatives (kind of like backpropagation!)
1. Forward pass: evaluate the function and store the intermediate values
2. Backward pass: evaluate the derivatives by traversing the computational graph in reverse order

I mean its not that different compared to forward mode.

**we start from the output**

## Expression Templates

Generate the expression in compile time using C++ templates!


## Performance?

Okay lets consider the number of operations required to evaluate the derivative of a function $f: \mathbb{R}^n \to \mathbb{R}$ with $m$ elementary operations.
- Dual numbers: $O(mn)$
- Forward mode AD: $O(mn)$
- Reverse mode AD: $O(m + n)$? (idk if this is correct)


Implementation specific performance: dual numbers are very cache friendly (no dynamic allocations!), but the forward mode AD requires a lot of memory. Reverse mode AD is even worse, but it is the most efficient in terms of number of operations.


