# First attempt

The parabola
$$
f(x)=ax^2 + bx + c
$$
has slope $\alpha$ at $x_0$ and its apex at $x_1$
$$
f'(x_0) = \alpha \\
f'(x_1)=0
$$
 Solving these for $a$ and $b$ we get
$$
\begin{align*}
a &= \frac{\alpha}{2(x_0-x_1)} \\
b &= \alpha \left(1 - \frac{x_0}{x_0-x_1}\right)  
\end{align*}
$$
The parabola has its right root at $x_2$
$$
x_2=\frac{-b+\sqrt{b^2 - 4ac}}{2a}
$$
Solving this for $c$ we get
$$
c = -ax_2^2 - bx_2
$$


Thus, the four parameters $(\alpha,x_0, x_1, x_2)$ are sufficient to determine the three parameters $(a,b,c)$ of the parabola. In general, three points $(x_0, y_0)$, $(x_1, y_1)$ and $(x_2, y_2)$ are needed to determine a parabola. In our derivation the parabola was determined by the conditions
$$
\begin{align*}
(x_0&, f'(x_0)  = \alpha) \\
(x_1&, f'(x_1) = 0) \\
(x_2&, f(x_2)  = 0)
\end{align*}
$$

# Second attempt

Consider a line and a parabola
$$
\begin{align}
f(x) &= \alpha x + \beta \\
g(x) &= ax^2 + bx +c
\end{align}
$$
The parabola is convex (i.e., $a$ is negative) and its left leg crosses the line at $x_0$ 
$$
\begin{align*}
f(x_0) &= g(x_0) &\iff \\
ax_0^2 + bx_0 + c &= \alpha x + \beta &\iff \\
ax_0^2 + (b-\alpha)x_0 + c - \beta &= 0
\end{align*}
$$
From this we can derive the point where they cross $(x_0, y_0)$
$$
\begin{align}
x_0 &= \frac{-b+\alpha  +\sqrt{(b-\alpha)^2 - 4a(c-\beta)} }{2a}  \\
y_0 &= \alpha x_0 + \beta
\end{align}
$$
We now add the constraints that the line and parabola have the same slope at $x_0$
$$
\begin{align}
f'(x) &= g'(x) \iff \\
\alpha &= 2ax_0 + b \iff \\
b &= \alpha - 2ax_0
\end{align}
$$
and that the parabola has its right root at $x_1$
$$
\begin{align}
g(x_1) &= 0 \iff \\
ax_1^2 + bx_1 + c &= 0
\end{align}
$$
Since we know that
$$
\begin{equation}
ax_0^2 + bx_0 + c = y_0
\end{equation}
$$
we can subtract both sides of eq. 11 from both sides of eq . 12 and get
$$
\begin{align*}
a(x_0^2 - x_1^2) + b(x_0-x_1) &= y_0 &\iff \\
a(x_0 - x_1)(x_0 + x_1) + b(x_0-x_1) &= y_0 &\iff \\
a(x_0 + x_1) + b &= \frac{y_0}{x_0-x_1} &\iff \\
a(x_0 + x_1) + \alpha - 2ax_0 &= \frac{y_0}{x_0-x_1} &\iff \\
a(x_0 + x_1 - 2x_0) + \alpha &= \frac{y_0}{x_0-x_1} &\iff \\
\end{align*}
$$

$$
\begin{equation}
a = \frac{\frac{y_0}{x_0-x_1} - \alpha}{x_1-x_0}
\end{equation}
$$

We now get $c$ easily from eq. 9.
$$
\begin{equation}
c = -ax_1^2 - bx_1
\end{equation}
$$
Hence from the paramers $(\alpha, \beta, x_1)$ we have derived the point were the line turns parabolic $x_0$ (eq. 3) and the parameters of the parabola $(a,b,c)$​ (eqs. 11, 7 and 12 ).

From Mathematica: 
$$
\begin{align}
x_0 &= -\frac{\beta + k}{\alpha } \\[8pt]
a   &= -\frac{\beta - k}{2 \left(x_1-x_2\right){}^2} \\[8pt]
b   &= \frac{x_1 \left(\beta -k+\alpha  x_1\right)}{\left(x_1-x_2\right){}^2} \\[8pt]
c   &= -\frac{\left(2 x_1-x_2\right) x_2 \left(\beta - k+\alpha  x_1\right)}{2 \left(x_1-x_2\right){}^2 }
\end{align}
$$
where
$$
\begin{equation}
k = \sqrt{\left(\beta +2 \alpha  x_1-\alpha  x_2\right) \left(\beta +\alpha  x_2\right)}
\end{equation}
$$
# DayDegrees

You can estimate the parameters of a day-degree model by regression the development rate $d$ (per day) on temperature:
$$
d = \frac{1}{L} = \lfloor \alpha T + \beta \rfloor_0
$$
where $L$  (days) is the average time needed to complete development at temperature $T$ and $b$ has a lower boundary of zero, which is reached below the lower threshold for development $T_0$ (oC),
$$
\alpha T_0 + \beta = 0 \iff T_0 = \frac{-\beta}{\alpha}
$$
Note the units of $\alpha$ (oC$^{-1}$ d$^{-1}$) and $\beta$ (d$^{-1}$).

Development can be modelled by rate summation of $d$ over time in increments of the time step ($\Delta t$) (d). The development has been completed once the sum reaches 1:
$$
\sum d\Delta t = 1 
$$
Alternatively, the summation can be formulated in terms of the temperature sum  $T_{sum}$ (oC dot d = oD),
$$
\sum \lfloor T-T_0 \rfloor_0 \Delta t = T_{sum}
$$
This equation follows if we recast the linear regression in terms of $T_0$ and $T_{sum}$, so that $d$ is expressed as proportional development in relation to $T_{sum}$
$$
d=\frac{\lfloor T-T_0 \rfloor_0}{T_{sum}}\quad\text{where}\quad T_{sum}=\frac{1}{\alpha}
$$
The `DayDegrees` model is specified by the inputs $T_0$ and optionally, the optimum temperature $T_{opt}$ (oC) and maximum temperature $T_{max}$ (oC)  for development

provides an out both for proportional development





However, a day-degree scale is usually parameterised, not by  the regression coefficients $(\alpha, \beta)$ but by the lower threshold for development $T_0$ (oC) and the duration in day-degrees $T_{sum}$ (oC dot d = oD), 
$$
\begin{align*}
\alpha T_0 + \beta = 0 &\iff T_0 = \frac{-\beta}{\alpha} \\[6pt]
\alpha T_{sum} = 1 &\iff T_{sum} = \frac{1}{\alpha}

\end{align*}
$$
