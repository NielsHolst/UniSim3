---
title: Saccharina
layout: default
---
$$
% Weight
\newcommand{\wSC} {\mathcal{S}^{C}}
\newcommand{\wSN} {\mathcal{S}^{N}}
\newcommand{\wSP} {\mathcal{S}^{P}}
\newcommand{\wSdw}{\mathcal{S}^{dw}}
\newcommand{\dwSdw}{\left<\mathcal{S}^{dw}\right>}
\newcommand{\wCC} {\mathcal{C}^{C}}
\newcommand{\wCdw}{\mathcal{C}^{dw}}
\newcommand{\dwCdw}{\left<\mathcal{C}^{dw}\right>}
\newcommand{\wNN} {\mathcal{N}^{N}}
\newcommand{\wNdw}{\mathcal{N}^{dw}}
\newcommand{\dwNdw}{\left<\mathcal{N}^{dw}\right>}
\newcommand{\wPP} {\mathcal{P}^{P}}
\newcommand{\wPdw}{\mathcal{P}^{dw}}
\newcommand{\dwPdw}{\left<\mathcal{P}^{dw}\right>}
\newcommand{\wWC} {\mathcal{W}^{C}}
\newcommand{\wWN} {\mathcal{W}^{N}}
\newcommand{\wWP} {\mathcal{W}^{P}}
\newcommand{\wWdw}{\mathcal{W}^{dw}}
\newcommand{\dwWdw}{\left<\mathcal{W}^{dw}\right>}
% Areas
\newcommand{\lai}{L_{ai}}
% Concentrations
\newcommand{\cSC} {[\mathcal{S}^{C}]}
\newcommand{\cSN} {[\mathcal{S}^{N}]}
\newcommand{\cSP} {[\mathcal{S}^{P}]}
\newcommand{\cCC} {[\mathcal{C}^{C}]}
\newcommand{\cNN} {[\mathcal{N}^{N}]}
\newcommand{\cPP} {[\mathcal{P}^{P}]}
\newcommand{\cWC} {[\mathcal{W}^{C}]}
\newcommand{\cWN} {[\mathcal{W}^{N}]}
\newcommand{\cWP} {[\mathcal{W}^{P}]}
\newcommand{\cWCmax} {[\mathcal{W}^{C}]_{max}}
\newcommand{\cWNmax} {[\mathcal{W}^{N}]_{max}}
\newcommand{\cWPmax} {[\mathcal{W}^{P}]_{max}}
% Proportions
\newcommand{\pSdw} {w_\mathcal{S}}
\newcommand{\pCdw} {w_\mathcal{C}}
\newcommand{\pNdw} {w_\mathcal{N}}
\newcommand{\pPdw} {w_\mathcal{P}}
% Demands
\newcommand{\dSC} {\left<\Delta\mathcal{S}^{C} \right>}
\newcommand{\dSN} {\left<\Delta\mathcal{S}^{N} \right>}
\newcommand{\dSP} {\left<\Delta\mathcal{S}^{P} \right>}
\newcommand{\dSdw}{\left<\Delta\mathcal{S}^{dw}\right>}
\newcommand{\dCC} {\left<\Delta\mathcal{C}^{C} \right>}
\newcommand{\dCN} {\left<\Delta\mathcal{C}^{N} \right>}
\newcommand{\dCdw}{\left<\Delta\mathcal{C}^{dw}\right>}
\newcommand{\dNC} {\left<\Delta\mathcal{N}^{C} \right>}
\newcommand{\dNN} {\left<\Delta\mathcal{N}^{N} \right>}
\newcommand{\dNdw}{\left<\Delta\mathcal{N}^{dw}\right>}
\newcommand{\dPP} {\left<\Delta\mathcal{P}^{P} \right>}
\newcommand{\dPdw}{\left<\Delta\mathcal{P}^{dw}\right>}
\newcommand{\dWC} {\left<\Delta\mathcal{W}^{C} \right>}
\newcommand{\dWN} {\left<\Delta\mathcal{W}^{N} \right>}
\newcommand{\dWP} {\left<\Delta\mathcal{W}^{P} \right>}
\newcommand{\dWdw}{\left<\Delta\mathcal{W}^{dw}\right>}
\newcommand{\dUC} {\left<\Delta\mathcal{U}^{C}\right>}
\newcommand{\dUN} {\left<\Delta\mathcal{U}^{N}\right>}
\newcommand{\dUP} {\left<\Delta\mathcal{U}^{P}\right>}
\newcommand{\dU}  {\left<\Delta\mathcal{U}\right>}
% Supplies
\newcommand{\sSC} {\Delta\mathcal{S}^{C}}
\newcommand{\sSN} {\Delta\mathcal{S}^{N}}
\newcommand{\sSCN}{\Delta\mathcal{S}^{CN}}
\newcommand{\sSdw}{\Delta\mathcal{S}^{dw}}
\newcommand{\sCC} {\Delta\mathcal{C}^{C}}
\newcommand{\sCN} {\Delta\mathcal{C}^{N}}
\newcommand{\sCdw}{\Delta\mathcal{C}^{dw}}
\newcommand{\sNC} {\Delta\mathcal{N}^{C}}
\newcommand{\sNN} {\Delta\mathcal{N}^{N}}
\newcommand{\sNdw}{\Delta\mathcal{N}^{dw}}
\newcommand{\sPP} {\Delta\mathcal{P}^{P}}
\newcommand{\sPdw}{\Delta\mathcal{P}^{dw}}
\newcommand{\sWC} {\Delta\mathcal{W}^{C}}
\newcommand{\sWN} {\Delta\mathcal{W}^{N}}
\newcommand{\sWP} {\Delta\mathcal{W}^{P}}
\newcommand{\sWdw}{\Delta\mathcal{W}^{dw}}
\newcommand{\sUC} {\Delta\mathcal{U}^{C}}
\newcommand{\sUN} {\Delta\mathcal{U}^{N}}
\newcommand{\sUP} {\Delta\mathcal{U}^{P}}
\newcommand{\sU}  {\Delta\mathcal{U}}
% Sources
\newcommand{\sRC} {\Delta\mathcal{R}^{C}}
\newcommand{\sRN} {\Delta\mathcal{R}^{N}}
\newcommand{\sRP} {\Delta\mathcal{R}^{P}}
% Expenditures
\newcommand{\dMC} {\left<\Delta\mathcal{M}^{C}\right>}
\newcommand{\dBC} {\left<\Delta\mathcal{B}^{C}\right>}
\newcommand{\dEC} {\left<\Delta\mathcal{E}^{C}\right>}
\newcommand{\dEN} {\left<\Delta\mathcal{E}^{N}\right>}
\newcommand{\dEP} {\left<\Delta\mathcal{E}^{P}\right>}
\newcommand{\sMC} {\Delta\mathcal{M}^{C}}
\newcommand{\sBC} {\Delta\mathcal{B}^{C}}
\newcommand{\sEC} {\Delta\mathcal{E}^{C}}
\newcommand{\sEN} {\Delta\mathcal{E}^{N}}
\newcommand{\sEP} {\Delta\mathcal{E}^{P}}
\newcommand{\lambdaS} {\lambda_\mathcal{S}}
\newcommand{\lambdaC} {\lambda_\mathcal{C}}
\newcommand{\lambdaN} {\lambda_\mathcal{N}}
\newcommand{\lambdaP} {\lambda_\mathcal{P}}
\newcommand{\eps} {\varepsilon}
% Units
\newcommand{\gCm}  {\;\text{g C/m}}
\newcommand{\gCmd} {\;\text{g C/m/d}}
\newcommand{\gNm}  {\;\text{g N/m}}
\newcommand{\gNmd} {\;\text{g N/m/d}}
\newcommand{\gPm}  {\;\text{g P/m}}
\newcommand{\gPmd} {\;\text{g P/m/d}}
\newcommand{\gdwm}  {\;\text{g dw/m}}
\newcommand{\gdwmd} {\;\text{g dw/m/d}}
\newcommand{\sqm}   {\text{m}^2}
% Allocation
\newcommand{\take}   {\text{take}\;}
\newcommand{\ask}    {\text{ask for}\;}
\newcommand{\from}   {\quad\text{from}\quad}
\newcommand{\giving} {\quad\text{giving}\quad}
\newcommand{\iiff}     {\text{if}\quad}
\newcommand{\then}   {\quad\text{then}\quad}
\newcommand{\elsee}   {\text{else}\quad}
\newcommand{\wFC}   {\mathcal{F}^C}
\newcommand{\wFN}   {\mathcal{F}^N}
\newcommand{\sFC}   {\Delta\mathcal{F}^C}
\newcommand{\sFN}   {\Delta\mathcal{F}^N}
% SD ratios
\newcommand{\sdEC} {\phi^C_\mathcal{E}}
\newcommand{\sdEN} {\phi^N_\mathcal{E}}
\newcommand{\sdEP} {\phi^P_\mathcal{E}}
\newcommand{\sdS}  {\phi_\mathcal{S}}
\newcommand{\sdC}  {\phi_\mathcal{C}}
\newcommand{\sdN}  {\phi_\mathcal{N}}
\newcommand{\sdP}  {\phi_\mathcal{P}}
$$

# The saccharina model

*Niels Holst & Teis Boderskov, Aarhus University, 21 February 2023.*

## Mathematical notation
The bracketing functions
$$
\lceil x \rceil^a \\
\lfloor x \rfloor_b
$$
imposes either an upper bound of $a$ or a lower bound of $b$ on $x$.

Full brackets denote a concentration:
$$
[X]
$$
Sharp parentheses around a *rate variable* denote the value it would attain under ideal circumstances, i.e. it refers to a certain demand:
$$
\left< \Delta X \right>
$$
The same *rate variable* without the sharp parentheses denotes the realised rate, i.e. the supply rate attained; it cannot exceed the demand:
$$
\Delta X \le \left< \Delta X \right>
$$
A *state variable* in sharp parentheses denotes the value it would attain at the end of this time step under ideal circumstances, whereas the same state variable without the sharp parentheses refers to its value at the beginning of the time step:
$$
\left < X \right> = X + \left< \Delta X \right>
$$
At the end of the time step, all state variables are updated according to the realised rates (supplies):
$$
X \leftarrow X + \Delta X
$$
Superscripts are used to denote the units of variables:
$$
\begin{split}
&X^{dw} \;\;&&\gdwm\\
&X^C \;\;&&\gCm\\
&X^N \;\;&&\gNm\\
&X^P \;\;&&\gPm\\
\end{split}
$$

## Model structure

### Space and time

The spatial unit considered is one meter of line. Hence, state and rate variables are per m. Rates are per day as the model operates with a time step of $\Delta t=1$ d.

In every time step, the model is updated in the following steps:

1. Compute demands.
2. Compute supplies.
3. Allocate supplies.

### Biomass

The biomass is split into four plant compartments (structure, carbon store, nitrogen store and phosphorus store).  For each compartment, the model keeps track of the weight of carbon, nitrogen, phosphorus and total biomass:

| Biomass   | Carbon (g C/m) | Nitrogen (g N/m) | Phosphorus (g P/m) | Total (g dw/m) |
| -------------- | --------------------- | ----------------------- | ------------------------- | ------------------------- |
| Structure      | $\wSC$                 | $\wSN$                  | *n.a.*      | $\wSdw$                   |
| Carbon store   | $\wCC$                 | *n.a.*            | *n.a.*      | $\wCdw$                   |
| Nitrogen store | *n.a.*           | $\wNN$                  | *n.a.*            | $\wNdw$                   |
| Phosphorus store | *n.a.* | *n.a.* | $\wPP$ | $\wPdw$ |
| Whole plant    | $\wWC$ | $\wWN$  | $\wWP$ | $\wWdw$ |

Since the concentrations of carbon, nitrogen and phosphorus in the four compartments are assumed fixed (see below), the model needs only four primary state variables:

* Structural biomass $(\wSdw)$
* Carbon store biomass $(\wCdw)$
* Nitrogen store biomass $(\wNdw)$
* Phosphorus store biomass $(\wPdw)$

The whole-plant sums are 
$$
\begin{split}
\wWC&=\wSC+\wCC\\[6pt]
\wWN&=\wSN+\wNN\\[6pt]
\wWP&=\wPP\\[6pt]
\wWdw&=\wSdw+\wCdw+\wNdw+\wPdw
\end{split}
$$

### Concentrations

| Concentrations | Carbon (g C/g dw) | Nitrogen (g N/g dw) | Phosphorus (g P/g dw) |
| -------------- | ------------------------------- | ---------------------------------- | ---------------------------------- |
| Structure      | $\cSC=\wSC/\wSdw=0.21$          | $\cSN=\wSN/\wSdw=0.02$             | *n.a.* |
| Carbon store   | $\cCC=\wCC/\wCdw=0.47$          | *n.a.*          | *n.a.* |
| Nitrogen store | *n.a.*  | $\cNN=\wNN/\wNdw=0.37$             | *n.a.*       |
| Phosphorus store | *n.a.* | *n.a.* | $\cPP=\wPP/\wPdw=0.33$ |
| Whole plant    | $\cWC=\wWC/\wWdw$               | $\cWN=\wWN/\wWdw$                  | $\cWP=\wWP/\wWdw$ |
| Whole plant max. | $\cWCmax=0.35$ | $\cWNmax=0.05$ | $\cWPmax=0.009$ |

Note: For consistency we should define $\cSP>0$. This will not change the outputs of the current model.

Under non-limiting growing conditions, the plant will attain its maximum concentration of all three elements. We can infer the distribution of biomass among the three compartments that will then be attained. From the above we have
$$
\begin{equation}\begin{split}
\cWCmax &= \frac{\wSdw\cSC+\wCdw\cCC}{\wWdw} \\[6pt]
\cWNmax &= \frac{\wSdw\cSN+\wNdw\cNN}{\wWdw} \\[6pt]
\cWPmax &= \frac{\wPdw\cPP}{\wWdw} \\[6pt]
1 &= \wSdw+\wCdw+\wNdw+\wPdw
\end{split}\end{equation}
$$
Solving these four equations for the unknown biomasses, we get
$$
\begin{equation}\begin{split}
\wSdw &= \pSdw\wWdw \\[6pt]
\wCdw &= \pCdw\wWdw \\[6pt]
\wNdw &= \pNdw\wWdw \\[6pt]
\wPdw &= \pPdw\wWdw \\[6pt]
\pSdw &= \frac{\pSdw'}{k}=0.1861,\quad \pCdw = \frac{\pCdw'}{k}=0.6615,
         \quad \pNdw = \frac{\pNdw'}{k}=0.1251, \quad \pPdw = \frac{\cWPmax}{\cPP}=0.02727\\[6pt]
\pSdw' &= (\cCC\cNN\cWPmax - \cCC\cNN\cPP + \cCC\cPP\cWNmax + \cNN\cPP\cWCmax)/k\\[6pt]
\pCdw' &= (\cSC\cNN\cPP - \cSC\cNN\cWPmax - \cSC\cPP\cWNmax + \cSN\cPP\cWCmax - \cNN\cPP\cWCmax)/k\\[6pt]
\pNdw' &= (\cSC\cPP\cWNmax + \cCC\cSN\cPP - \cCC\cSN\cWPmax - \cCC\cPP\cWNmax - \cSN\cPP\cWCmax)/k\\[6pt]
k      &= \cSC\cNN\cPP + \cCC\cSN\cPP - \cCC\cNN\cPP
\end{split}\label{eq_ideal_proportions}\end{equation}
$$

So, under non-limiting conditions the plant biomass will constitute roughly one fifth structure $(\pSdw)$ , two-thirds  carbon store $(\pCdw)$, one eighth nitrogen store $(\pNdw)$, and 3% phosphorus store $(\pPdw)$.  

### Growth: demands and supplies

In every time step, the plant's demand for growth, i.e. its biomass increment $\dWdw$ under non-limiting conditions,  is calculated. The specific demands per compartment are formulated further down.

| Growth demands | Carbon (g C/m) | Nitrogen (g N/m) | Phosphorus (g P/m) | Total (g dw/m) |
| -------------- | ---------------------   | ----------------------- | ------------------------- | ------------------------- |
| Structure      | $\dSC$                  | $\dSN$                  | *n.a.* | $\dSdw$                   |
| Carbon store   | $\dCC$                  | *n.a.*            | *n.a.*            | $\dCdw$                   |
| Nitrogen store | *n.a.*            | $\dNN$                  | *n.a.*            | $\dNdw$                   |
| Phosphorus store | *n.a.* | *n.a.* | $\dPP$ | $\dPdw$ |
| Whole plant    | $\dWC$ | $\dWN$ | $\dWP$ | $\dWdw$ |

The whole-plant sums of demands are
$$
\begin{split}
\dWC&=\dSC+\dCC \\[6pt]
\dWN&=\dSN+\dNN \\[6pt]
\dWP&=\dPP \\[6pt]
\dWdw&=\dSdw+\dCdw+\dNdw+\dPdw
\end{split}
$$
The resources acquired by photosynthesis and nutrient uptake result in supplies, which are equal to or less than the demands:

| Growth supplies | Carbon (g C/m) | Nitrogen (g N/m) | Phosphorus (g P/m) | Total (g dw/m) |
| -------------- | ---------------------   | ----------------------- | ------------------------- | ------------------------- |
| Structure      | $\sSC$                  | $\sSN$                  | *n.a.* | $\sSdw$             |
| Carbon store   | $\sCC$                  | *n.a.*   | *n.a.*            | $\sCdw$                   |
| Nitrogen store | *n.a.*            | $\sNN$                  | *n.a.*            | $\sNdw$                   |
| Phosphorus store | *n.a.* | *n.a.* | $\sPP$ | $\sPdw$ |
| Whole plant    | $\sWC$ | $\sWN$ | $\sWP$ | $\sWdw$ |

The whole-plant sums of supplies are
$$
\begin{split}
\sWC&=\sSC+\sCC \\[6pt]
\sWN&=\sSN+\sNN \\[6pt]
\sWP&=\sPP \\[6pt]
\sWdw&=\sSdw+\sCdw+\sNdw+\sPdw
\end{split}
$$

### Expenditures: demands and supplies

Resources are needed not only for growth but also for basic metabolism, exudation and tissue building costs:

| Expenditures         | Demand         | Supply         |
| -------------------- | -------------- | -------------- |
| Basic metabolism     | $\dMC$ (g C/m) | $\sMC$ (g C/m) |
| Carbon exudation     | $\dEC$ (g C/m) | $\sEC$ (g C/m) |
| Nitrogen exudation   | $\dEN$ (g N/m) | $\sEN$ (g N/m) |
| Phosphorus exudation | $\dEP$ (g P/m) | $\sEP$ (g P/m) |
| Building costs       | $\dBC$ (g C/m) | $\sBC$ (g C/m) |

### Resource acquisition

The summed demands for growth and expenditures define the total daily demands for the uptake of carbon $\dUC$ (g C/m), nitrogen $\dUN$ (g N/m) and phosphorus $\dUP$ (g P/m). They follow from the definitions above  as
$$
\begin{equation}
\begin{split}
\dUC &= \dSC &&+ \dCC &&+\dEC+\dMC +\dBC  \\[6pt]
\dUN &= \dSN &&+ \dNN &&+\dEN\\[6pt]
\dUP &= \dPP &&+\dEP
\end{split}
\label{totaldemands}
\end{equation}
$$

Resources are acquired from the water surrounding the plant canopy. This is modelled as three separate processes, one for each element, in the form of the Baumgärtner-Gutierrez functional response,
$$
\begin{equation}
\sU = \dU
\left\{ 1-\text{exp}\left(-\frac{aXY \Delta t}{\dU} \right) \right\}
\label{eq_b_g_response}
\end{equation}
$$
where $\sU$ is the realised daily supply with respect to the demand $\dU$. The acquisition efficiency $a$ is  applied to the product of resource density $X$ and plant density $Y$. The units of the parameters and variables differ whether eq. $\ref{eq_b_g_response}$ is applied to the uptake of carbon, nitrogen or phosphorus. The relation has the shape of a saturation curve, which is linear under meagre conditions (small $XY$) with a slope of $aY\Delta t$; it approaches the demand $\dU$ when conditions are rich (large $XY$):

<img src="../../media/models/saccharina/b-g-resp.png" style="zoom:50%;" />

The resource variable $X$ is sunlight intensity for carbon acquisition and water concentrations of dissolved nitrogen and phosphorus for the other two elements. The uptake efficiency $a$ takes care of converting the units of $X$ into $\sU$. 

Equations for demands and supplies are formulated in the following. Since supplies cannot be expected to fulfill the demands, supplies must subsequently be allocated to meet specific demands according to a priority scheme.

## Demands

Empirically, plant growth is measured as an increase in biomass. This is due to an increase in one or more of the four plant compartments. If the plant is growing under unlimiting conditions then the plant compartments will maintain their optimum proportions (eq. $\ref{eq_ideal_proportions}$) during growth, and they will thus all grow at the same maximum growth rate ($g$; g dw/g dw/d). The growth demand is calculated first for structural growth. Afterwards the growth demand of the stores are calculated, so that the optimum proportions will be achieved if resources are plentiful.

### Structural growth

The biomass demand for structural growth is
$$
\begin{equation}
\dSdw =\varphi_T\,\varphi_{sal}\,g\,\wSdw\Delta t
\label{eq_dSdw}
\end{equation}
$$
where $g$ (g dw/g dw/d) is the maximum growth rate with the scaling factors ($\varphi_T,\,\varphi_{sal}\in[0;1]$) taking sub-optimal conditions into account. The corresponding  elementary demands (used in eq. $\ref{totaldemands}$) are
$$
\begin{split}
\dSC &= \cSC \dSdw \\[6pt]
\dSN &= \cSN \dSdw 
\end{split}
$$

The temperature response ($\varphi_T$) is piece-wise linear with two breakpoints:

<img src="../../media/models/saccharina/T-response.png" style="zoom:8%;" />

The salinity response ($\varphi_{sal}$) is likewise piece-wise linear with two breakpoints, the first one at 50% response:

<img src="../../media/models/saccharina/sal-response.png" style="zoom:8%;" />

### Stores

The demands for the storage of carbon $\dCC$, nitrogen $\dNN$ and phosphorus $\dPP$ are linked to the current whole-plant biomass $\wWdw$ by the ideal plant proportions achieved under non-limiting conditions (eq. $\ref{eq_ideal_proportions}$). We have
$$
\begin{equation}
\begin{split}
\dCC &= \cCC\,\left\lfloor\pCdw\wWdw \;-\; \wCdw\right\rfloor_0 \\[6pt]
\dNN &= \cNN\,\left\lfloor\pNdw\wWdw \;-\; \wNdw\right\rfloor_0 \\[6pt]
\dPP &= \cPP\,\left\lfloor\pPdw\wWdw \;-\; \wPdw\right\rfloor_0 
\end{split}
\label{eq_store_demands_dw}
\end{equation}
$$
For each demand, the first term represents the ideal dry weight of the store in proportion to $\wWdw$, while the second term is the current dry weight of the store. A floor value of zero is imposed in case of superflous stores. The demands are used in eq. $\ref{totaldemands}$.

### Maintenance respiration

The maintenance (i.e., dark) respiration was measured at 20℃ in the laboratory by Davison et al. (1991) and Nepper-Davidsen et al. (2019) who found, respectively, values of 1.0 and 1.5 μmol O~2~ /g fw/h. We can convert these into g glucose/g dw by the conversion ratio,
$$
1 \frac{\mu\text{mol O}_2}{\text{g fw } \times \text{ h}} \times
\frac{1\text{ g fw}}{0.09\text{ g dw}} \times
\frac{180\text{ g dw/mol glucose}}{6 \text{ mol O}_2 \text{/mol glucose}} \times
\frac{24\text{ h}}{1 \text{ d}} =0.00800 \frac{\text{g glucose}}{\text{g dw}\times\text{d}}
$$
The above estimates at 1.0 and 1.5 μmol O~2~ /g fw/h give us a respiration rate at 20℃ ($r_{20}$) in the range,
$$
r_{20} \in [8;12] \frac{\text{mg glucose}}{\text{g dw }\times\text{ d}}
$$
which matches the range of 7-13 mg glucose/g dw/d typical of crop leaves (de Wit 1978, p. 51). We adjust this respiration rate by temperature assuming $Q_{10}$ = 1.05 (Davison et al. 1991), which gives us the temperature-dependent respitation rate ($r_T$; g glucose/g dw/d ),
$$
r_T=r_{20}Q_{10}^{(T-20)/10}
$$
The daily carbon demand for maintenance respiration $\dMC$ (g C/m) then becomes
$$
\dMC = \frac{72\text{ g C}}{180\text{ g glucose}}\times
r_T\, \wWdw\Delta t
$$

### Exudation cost

The mechanisms of exudation in kelp are not well understood. Abdullah and Fredriksen (2004) estimated the exudation rate of *Laminaria* *hyperborea* and found some correlation with the growth rate (r=0.73) with an annual average of 26% exudation of the fixed carbon. Earlier on, Newell et al. (1980) found that the dry matter contents of the exudate in two other kelp species were 10% carbohydrate, 3% protein and 0.3% lipids (the remaining, presumably, consisted of salts).

We assume that the exudation rates of carbon, nitrogen and phosphorus (at rates of $\epsilon^C$, $\epsilon^N$ and $\epsilon^P$, respectively, all per day)  all pertain to the total biomass,
$$
\begin{equation}
\begin{split}
\dEC &= \epsilon^C\wWdw\Delta t\\[6pt]
\dEN &= \epsilon^N\wWdw\Delta t\\[6pt]
\dEP &= \epsilon^P\wWdw\Delta t
\end{split}
\label{eq_exudation}
\end{equation}
$$
In effect, exudation is a daily tax on the standing biomass, just like maintenance respiration.

### Building costs

The building costs (also known as growth respiration) for different plant organs depend on their chemical composition (Penning de Vries et al. 1974). We will use the general costs tabulated by de Wit (1978): 0.30 g glucose/g dw for structural biomass , 0.14 g glucose/g dw for carbon reserves and 0.41 g glucose/g dw for nitrogen reserves, the latter being the average value for nitrogen uptaken as ammonia or nitrate. De Wit mentions (1978, p.50) that the cost of mineral uptake is minimal. 

We get the following conversions costs,
$$
\begin{equation}
\begin{split}
\lambdaS &= 0.30\frac{\text{ g glucose}}{\text{g dw}}\times
\frac{72\text{ g C}}{180\text{ g glucose}}=0.120\frac{\text{g C}}{\text{g dw}} \\[6pt]
\lambdaC &= 0.14\frac{\text{ g glucose}}{\text{g dw}}\times
\frac{72\text{ g C}}{180\text{ g glucose}}=0.0560\frac{\text{g C}}{\text{g dw}} \\[6pt]
\lambdaN &= 0.41\frac{\text{ g glucose}}{\text{g dw}}\times
\frac{72\text{ g C}}{180\text{ g glucose}}=0.164\frac{\text{g C}}{\text{g dw}} \\[6pt]
\lambdaP &= 0\,\frac{\text{g C}}{\text{g dw}}
\end{split}
\label{conversion_costs}
\end{equation}
$$
The total daily carbon demand to cover building costs $\dBC$ (g C/m) is then
$$
\begin{equation}
\dBC = \lambdaS\dSdw +\lambdaC\dCdw +\lambdaN\dNdw
\label{eq_building_cost}
\end{equation}
$$

## Supplies

### Carbon supply

A part of the incoming sunlight irradiation ($I_0$; mol PAR/m^2^/d) is reflected at the water surface, for which we assume a fixed reflectivity ($\rho$ = 0.05; see Kirk 2010, section 2.5). Light is absorbed as it passes down the water column. We assume an exponential decay characterised by the extinction coefficient $k_d$, which varies with the clarity of the water.  Thus the light available at depth $d$ ($I_d$; mol PAR/m^2^/d) becomes
$$
I_d=(1-\rho) exp(-k_dd)I_0
$$

The canopy will span over a time-varying range of water depths. It is anchored at the depth of the line but is otherwise free-floating with changing currents. To keep the model at a level of detail reflecting our knowledge, we assume that the canopy is exposed to $I_d$ at a fixed depth $d$.

To model the photosynthetic rate of a single plant, Benjamin and Park (2007) defined two allometric relations depending on plant biomass ($M$), one for leaf area,
$$
A_l = c_1 M^{c_2}
$$
the other for the crown zone area, i.e. the horizontal area covered by the plant crown,
$$
A_z = c_3M^{c_4}
$$
They fixed two of the parameters $c_2=1$ and $c_4=2/3$ but acknowledged that all four parameters ought to be estimated experimentally according to plant species and growing conditions.

Consider these calibration data:

```
      Date Wtotal     A    L Cconc  Nconc
22-01-2020   7.21  0.58 0.08 0.250 0.0495
27-02-2020  50.99  3.26 0.23 0.252 0.0522
26-03-2020 160.79  4.88 0.38 0.296 0.0335
28-04-2020 587.63 11.37 0.72 0.364 0.0136
03-06-2020 834.03 12.63 0.76 0.351 0.0111
```

The columns `Wtotal`, `Cconc` and `Nconc` correspond to the variables $\wWdw$, $\cWC$ and $\cWN$. For the columns `A` and `L` we define the variables: leaf area $A_l$ (m^2^/m) and leaf length $L$ (m).  We will assume that the status of the stores do not affect leaf area or length. Thus in our model, $A_l$ and $L$  depend on the structural biomass $\wSdw$ not the total biomass $\wWdw$. Structural biomass is not directly observable but we can derive it, knowing the carbon and nitrogen concentrations. We've got 
$$
\begin{split}
\cWC &= \frac{\cSC\wSdw+\cCC\wCdw}{\wWdw} \quad\and\quad 
\cWN = \frac{\cSN\wSdw+\cNN\wNdw}{\wWdw} \quad\and\quad 
\wWdw = \wSdw+\wCdw+\wNdw
\quad\implies \\[6pt]
\wSdw &= \wWdw\frac{\cCC \left(\cWN - \cNN\right) \;+\; \cNN \cWC}{\cCC \left(\cSN - \cNN\right) \;+\; \cNN\cSC}
\end{split}
$$
from which we can add another column `Wstruct` as an estimate of $\wSdw$:

```
      Date Wtotal     A    L Cconc  Nconc Wstruct
22-01-2020   7.21  0.58 0.08 0.250 0.0495    4.83
27-02-2020  50.99  3.26 0.23 0.252 0.0522   32.87
26-03-2020 160.79  4.88 0.38 0.296 0.0335   89.94
28-04-2020 587.63 11.37 0.72 0.364 0.0136  221.41
03-06-2020 834.03 12.63 0.76 0.351 0.0111  371.66
```

Linear regression on log-transformed data let us predict both $A_l$ and $L$ from $\wSdw$,
$$
\begin{equation}
\begin{split}
A_l &= c_A(\wSdw)^{e_A} &&= 0.211(\wSdw)^{0.716} \\[6pt]
L &= c_L(\wSdw)^{e_L}   &&= 0.0358(\wSdw)^{0.531} \\[6pt]
\end{split}
\label{eq_leaf}
\end{equation}
$$
A cross section of the canopy is roughly wedge-shaped (i.e. pizza slice-shaped) with its point fastened at the line suspending the canopy. Its maximum horizontal extent will be $2L$ and its minimum $1L$. Sunlight will hit the canopy surface from all sides except from the bottom (in Danish waters, upwards scattering of light is neglible and the bottom is either far down or of low albedo). Thus we arrive at the crown zone area $A_z$ (m^2^/m) ,
$$
\begin{equation}
A_z = c_ZL
\label{eq_crown_zone}
\end{equation}
$$
with $c_Z\in[1;2]$ m/m.

We compute the leaf area index $A_i$ (m^2^ leaf/m^2^ crown zone) following Benjamin and Park (2007),
$$
A_i=\frac{A_l}{A_z}
$$
It follows that the canopy is exposed to a light intensity of $A_zI_d$ (mol PAR/m/d). The units are derived as
$$
\frac{\sqm \text{ crown zone}}{\text{m}}\cdot\frac{\text{mol PAR}}{\sqm \text{ sea surface}\cdot \text{d}}=
\frac{\text{mol PAR}}{\text{m}\cdot \text{d}}
$$
The two measures of area may seem incommensurable at first glance. That they are the same is most easily seen if one considers the crown zone area in its maximum extent. This occurs when it is spread out horizontally and reaches its maximum width of $2L$. Thus, with $L=0.8$ m the crown zone would cover $1.6$ m^2^ sea surface per m line. This would be the area exposed to the incoming sunlight $I_d$. With $c_Z<2$ (eq. $\ref{eq_crown_zone}$) the exposed area would be less, and at the minimum $s_Z=1$ it would be only $0.8$ m^2^/m.

The incoming light $A_zI_d$ is intercepted by the plant canopy according to $A_i$ and the leaf light extinction coefficient $k_l$. The fraction of light intercepted is $1-exp(-k_lA_i)$, and the amount of light absorbed $I_a$ (mol PAR/m/d) becomes
$$
I_a = A_zI_d\{1-exp(-k_lA_i)\}
$$
The relations all ultimately depend on $\wSdw$ as illustrated here with $c_Z=2$ m and $k_l=0.7$:

![carbon-supply](..\media\models\saccharina\carbon-supply.png)

The leaf area index is always high and the fraction of light intercepted (`fabs` in the figure) close to 100%. This means that the extent of the crown zone $A_z$ is more decisive for light absorption than the leaf area $A_l$, and that $c_Z$ is more influential than $k_l$, which we can safely leave at $k_l=0.7$. According to this reasoning, line-grown plants are expected to be morphologically distinguishable from solitary plants, due to the dense foliage, with narrower and maybe thinner leaves (showing etiolated growth).

$I_a$ corresponds to the resource density $X$ in eq. $\ref{eq_b_g_response}$, where we plug it in:
$$
\begin{equation}
\sUC=\dUC\left\{1-exp\left( 
  -\frac{\alpha\,\psi^N I_a\Delta t}
  {\dUC}
\right) \right\}
\label{eq_sUC}
\end{equation}
$$
The photosynthetic efficiency $\alpha$ (g C/mol PAR) converts the absorbed light into fixed carbon. It is modulated by $\psi ^N\in [0;1]$, which takes into account sub-optimal concentrations of plant nitrogen $\cWN$. 

The value of $\alpha$ can be estimated from Sogn Andersen et al. (2013, Fig. 3):
$$
\begin{split}
\alpha &= 0.23\frac{\mu\text{mol O}_2}{\text{g fw}\cdot\text{h}} / \frac{\mu\text{mol PAR}}{\text{m}^2\cdot\text{s}} \\[6pt]
&=0.23\cdot421.5\frac{\mu\text{g C}}{\text{dm}^2\cdot\text{h}} / \frac{\mu\text{mol PAR}}{\text{m}^2\cdot\text{s}} \\[6pt]
&=0.969\cdot10^{-5}\frac{\text{g C}}{\text{dm}^2\cdot\text{h}} / \frac{\mu\text{mol PAR}}{\text{m}^2\cdot\text{s}} \\[6pt]
&=0.969\cdot10^{-5}\frac{100}{3600}\frac{\text{g C}}{\mu\text{mol PAR}} \\[6pt]
&=2.69\frac{\text{g C}}{\text{mol PAR}}
\end{split}
$$
The response ($\psi^N$) to plant nitrogen forms a piece-wise linear response with two breakpoints, 

<img src="../../media/models/saccharina/N-response.png" style="zoom:8%;" />

### Nitrogen and phosphorus supply

We apply the acquistion function (eq. $\ref{eq_b_g_response}$) to the uptake of nitrogen ($\sUN$; g N/m) and phosphorus ($\sUP$; g P/m) from the surrounding sea water, where it occurs at concentrations  $[N_{water}]$ and $[P_{water}]$ (both $\mu$M);
$$
\begin{equation}
\begin{split}
\sUN &= \dUN
\left\{ 1-\text{exp}\left(-\frac{\beta\, [N_{water}]A_l\Delta t }{\dUN} \right) \right\} \\[6pt]
\sUP &= \dUP
\left\{ 1-\text{exp}\left(-\frac{\gamma\,[P_{water}]A_l\Delta t }{\dUP} \right) \right\} \\[6pt]
\end{split}
\label{eq_NP_supply}
\end{equation}
$$

with nitrogen acquisition efficiency $\beta$ in units,
$$
\frac{\text{g N}}{\text{m}^2\text{/m}\cdot \mu\text{M N}\cdot\text{d}}
$$
and phosphorus acquisition efficiency $\gamma$ in units,
$$
\frac{\text{g P}}{\text{m}^2\text{/m}\cdot \mu\text{M P}\cdot\text{d}}
$$
These acquisition functions (eq. $\ref{eq_NP_supply}$) are simpler than the one for carbon (eq. $\ref{eq_sUC}$). This is because we assume that the concentration of dissolved nitrogen and phosphorus, unlike light, is not attenuated as water passes through the canopy. Hence, for these nutrients we base uptake on leaf area $A_l$ rather than leaf area index $A_i$. There will, locally, be a slight drop in nutrient concentrations as they are taken up by the plants but we assume that water currents will quickly reinstate the ambient concentrations.

## Allocation

The acquired carbon, nitrogen and phosphorus is allocated in a sequence of steps, according to demands, while stores are being mobilised as needed. The allocation procedure follows a prioritised sequence of the demands (*cf.* Gutierrez 1996): 

1. Maintenance respiration
2. Exudation
3. Plant structure
4. Nitrogen store
5. Carbon store 
6. Phosphorus store

The first two steps allocate ressources to obligatory expenditures. The costs for building tissue of plant structure and stores are taken into account during allocation in proportion to the actual supplies. We assume that nitrogen stores take priority over carbon stores; this will only have a consequence if carbon to cover store building costs are limiting. The phosphorus store is built at no cost ($\lambdaP=0$, eq. $\ref{conversion_costs}$).

During each allocation step, priority is given to the resources just acquired: $\dUC$, $\dUN$ and $\dUP$. Only when these have been exhausted are stores ($\wCC$, $\wNN$ and $\wPP$) being mobilised to meet demands.

The `allocation.R` script demonstrates the logic applied during allocation by the function `take`. In step 1, for example, when carbon for maintenance respiration is allocated, it is written like this
$$
\take\dMC\from\sUC,\wCC \giving\sMC
$$
This corresponds to a call of the `take` function with `demand` equal to $\dMC$, `sources` equal to the vector $(\sUC,\wCC)$, and with the function result (local variable `supply`) assigned to $\sMC$.

Note that the function has side effects (recognised by operator `<<-` in the R script) as it reduces the resources from which the supply has been taken. 

```R
take = function(demand, sources) {
  supply = 0
  lacking = demand
  # Go through sources in order of priority
  for (source in sources) {
    lacking = demand - supply
    taken = if (lacking > R[source]) R[source] else lacking
    supply = supply + taken
    R[source] <<- R[source] - taken
    # Stop if supply meets demand
    if (abs(supply - demand) < 1e-6) break
  }
  supply
}
```

In the following, `ask for` has the same functionality as `take`, except that it calculates the supply without any side effects. It is used to ask what the supply *would be*, in case it were taken from the given sources.

### Step 1. Maintenance respiration

First, carbon needed for respiration is taken,
$$
\take\dMC\from\sUC,\wCC \giving\sMC
$$

If the uptaken carbon $\sUC$ was not sufficient to meet the demand $\dMC$ then the deficit was drawn from the carbon store $\wCC$ by the `take` function. We must keep track of the carbon store before and after allocation to calculate any loss to the store ($\sCdw_1$; g dw/m) in this step,
$$
\sCdw_1 = \frac{\wCC_{after} - \wCC_{before}}{\cCC} \le 0
$$
If the carbon store was exhausted without the demand being fulfilled, we need to recycle structural biomass $\sSdw_1$ (g dw/m) to cover the deficit:
$$
\begin{equation}
\sSdw_1= \left\lceil\frac{\sMC-\dMC}{\cSC}\right\rceil^0
\label{eq_maintenance_deficit}
\end{equation}
$$
If $\sSdw\lt 0$ then steps 2 to 6 below are skipped and all remaining allocations set to zero, i.e. $\sSdw_{i>1}=0$, $\sCdw_{i>1}=0$, $\sNdw_{i>1}=0$ and $\sPdw_{i>1}=0$.

On exit from step 1:

* The demand for maintenance respiration has been met.
* Resources $\sUC$ and $\wCC$ may have decreased.
* If $\wCC$ decreased then carbon store dry matter was lost, $\sCdw_1 \lt 0$.
* Structural mass may have been lost and carbon resources emptied, $\sSdw_1<0 \and \sUC=0 \and \wCC=0$. 

### Step 2. Exudation

We take carbon, nitrogen and phosphorus exudates from the following sources,
$$
\begin{split}
\take\dEC&\from\sUC,\wCC &\giving\sEC\\[6pt]
\take\dEN&\from\sUN,\wNN &\giving\sEN\\[6pt]
\take\dEP&\from\sUP,\wWP &\giving\sEP
\end{split}
$$
If any resource is limiting, i.e if $\sEC<\dEC$ or $\sEN<\dEN$ or $\sEP<\dEP$, then structural biomass will not be recycled to make up for the lack. This is in contrast to basic metabolism in step 1, which was strictly obligatory. Thus stressed plants might exude less than demanded. We define the supply/demand ratios for exudation (which are useful descriptors of this aspect of plant performance) by
$$
\begin{split}
\sdEC&=\frac{\sEC}{\dEC} \in [0;1] \\[6pt]
\sdEN&=\frac{\sEN}{\dEN} \in [0;1] \\[6pt]
\sdEP&=\frac{\sEP}{\dEP} \in [0;1] 
\end{split}
$$
Any losses to stores in step 2 are recorded (in g dw/m) as
$$
\begin{split}
\sCdw_2 &= \frac{\wCC_{after} - \wCC_{before}}{\cCC}\le 0\\[6pt]
\sNdw_2 &= \frac{\wNN_{after} - \wNN_{before}}{\cNN}\le 0\\[6pt]
\sPdw_2 &= \frac{\wPP_{after} - \wPP_{before}}{\cPP}\le 0
\end{split}
$$
On exit from step 2:

* The demands for exudation may not have been met: $\sdEC\le1 \and \sdEN\le1 \and \sdEP\le1$.
* Resources $\sUC,\wCC,\sUN,\wNN,\sUP$ and $\wWP$  may have decreased.
* For any store $\wCC$, $\wNN$ or $\wPP$ that decreased, store dry matter was lost, respectively, $\sCdw_2 \lt 0$, $\sNdw_2 \lt 0$ or $\sPdw_2 \lt 0$.

### Step 3. Plant structure

Structural growth uses carbon and nitrogen for structural tissue plus carbon to cover the building costs. To find out which resource is more limiting for structural growth, carbon or nitrogen, we tentatively ask for the allocation of both. For the carbon demand, we add the building costs,
$$
\begin{split}
&\ask\dSC\left(1+\frac{\lambdaS}{\cSC}\right)&&\from\sUC,\wCC &&\giving\sSC_0\left(1+\frac{\lambdaS}{\cSC}\right) \\[6pt]
&\ask\dSN&&\from\sUN,\wNN &&\giving\sSN_0
\end{split}
$$
We  convert the tentative supplies $\sSC_0$ and $\sSN_0$ to the corresponding biomass decrement. The supply that gives the smaller increment governs the outcome:
$$
\begin{split}
&\iiff\frac{\sSC_0}{\cSC}<\frac{\sSN_0}{\cSN} \then \\[6pt]
&\quad\take\dSC\left(1+\frac{\lambdaS}{\cSC}\right)&&\from\sUC,\wCC&&\giving\sSC\left(1+\frac{\lambdaS}{\cSC}\right) \\[6pt]
&\quad\take\frac{\cSN}{\cSC}\sSC&&\from\sUN,\wNN &&\giving\sSN \\[6pt]
&\elsee\\[6pt]
&\quad\take\dSN&&\from\sUN,\wNN &&\giving\sSN \\[6pt]
&\quad\take\frac{\cSC}{\cSN}\sSN\left(1+\frac{\lambdaS}{\cSC}\right)&&\from\sUC,\wCC &&\giving\sSC\left(1+\frac{\lambdaS}{\cSC}\right)
\end{split}
$$

In the first case, carbon is the more limiting and carbon is allocated first. Afterwards, the nitrogen needed to follow with the carbon to build structural tissue is allocated. In the alternative case, the order of allocation is the opposite.

The allocation to structural growth (g dw/m) in step 3 is
$$
\sSdw_3=\frac{\sSC}{\cSC}=\frac{\sSN}{\cSN} \ge 0
$$

resulting in the supply/demand ratio for structural growth,
$$
\sdS=\frac{\sSdw_3}{\dSdw} \in [0;1]
$$
Any losses to stores (g dw/m) in step 3 are
$$
\begin{split}
\sCdw_3 &= \frac{\wCC_{after} - \wCC_{before}}{\cCC} \le 0\\[6pt]
\sNdw_3 &= \frac{\wNN_{after} - \wNN_{before}}{\cNN} \le 0
\end{split}
$$
On exit from step 3:

* The demand for structural growth may not have been met, $\sdS\le1$.
* Resources $\sUC,\wCC,\sUN$ and $\wNN$ may have decreased.
* For any store $\wCC$ or $\wNN$ that decreased, store dry matter was lost, respectively, $\sCdw_3 \lt 0$ or $\sNdw_3 \lt 0$.

### Step 4. Nitrogen store

We need nitrogen for the store itself plus carbon to cover the building costs. This means that both nitrogen and carbon could be limiting. As in step 3, we tentatively ask for the allocation of both,
$$
\begin{split}
&\ask\dNN&&\from\sUN &&\giving\sNN_N \\[6pt]
&\ask\lambdaN\frac{\dNN}{\cNN}&&\from\sUC,\wCC &&\giving\lambdaN\frac{\sNN_C}{\cNN}
\end{split}
$$
First, we ask for the demanded nitrogen $\dNN$ to be covered by the nitrogen that was just uptaken. As a result we get the nitrogen-limited supply $\sNN_N \le \dNN$.

Second, we ask for the carbon needed to build the tissue demanded by the nitrogen store. We find the building cost by converting the nitrogen demand $\dNN$ (g N/m) to a biomass demand (g dw/m), using the nitrogen concentration in the store $\cNN$ (g N/g dw), and then convert that to a carbon demand using the building cost $\lambdaN$ (g C/g dw). The demanded carbon may be sourced from the just fixated carbon $\sUC$, as well as from the carbon store $\wCC$. As a result we get the carbon-limited supply $\sNN_C \le \dNN$.

We pick the source that is more limiting and take the appropriate amounts of both nitrogen and carbon:
$$
\begin{split}
&\iiff \sNN_N < \sNN_C &&\then \\[6pt]

&\quad\take\dNN&&\from\sUN &&\giving\sNN \\[6pt]
&\quad\take\lambdaN\frac{\sNN}{\cNN}&&\from\sUC,\wCC \\[6pt]

&\elsee\\[6pt]

&\quad\take\lambdaN\frac{\dNN}{\cNN}&&\from\sUC,\wCC &&\giving\lambdaN\frac{\sNN}{\cNN} \\[6pt]
&\quad\take\sNN&&\from\sUN 
\end{split}
$$

In the first case, nitrogen is the more limiting. Consequently, we take the available nitrogen first to get the supply $\sNN$. Next we calculate the carbon expenditure needed to build $\sNN$ and take that from the available carbon. The second case operates in the same way, just the other way around.

Allocation to to the nitrogen store (g dw/m) in step 4 is
$$
\sNdw_4 = \frac{\sNN}{\cNN} \ge 0
$$

resulting in the supply/demand ratio for stored nitrogen,
$$
\sdN=\frac{\sNdw_4}{\dNdw} \in [0;1]
$$
Any loss to the carbon store (g dw/m) in step 4 is
$$
\sCdw_4 = \frac{\wCC_{after} - \wCC_{before}}{\cCC} \le 0
$$


On exit from step 4:

* The demand of the nitrogen store may not have been met, $\sdN \le1$.
* resources $\sUC,\wCC$ and $\sUN$ may have decreased.
* If  $\wCC$ decreased, carbon store dry matter was lost, $\sCdw_4 \lt 0$.

### Step 5. Carbon store

The supply to the carbon store and the associated building cost can only be taken from the just fixated carbon,
$$
\take\dCC\left(1+ \frac{\lambdaC}{\cCC}\right)\from\sUC\giving\sCC\left(1+\frac{\lambdaC}{\cCC}\right)
$$

Allocation to the carbon store (g dw/m) in step 5 is then
$$
\sCdw_5 = \frac{\sCC}{\cCC}
$$
resulting in the supply/demand ratio for stored carbon,
$$
\sdC=\frac{\sCdw_5}{\dCdw} \in [0;1]
$$
On exit from step 5:

* The demand of the carbon store may not have been met, $\sdC \le1$
* $\sUC$ may have decreased.

### Step 6. Phosphorus store

There is only one source to supply the phosphorus store and there is no building cost,
$$
\take\dPP\from\sUP\giving\sPP
$$
Allocation to the phosphorus store (g dw/m) in step 6 is then
$$
\sPdw_6 = \frac{\sPP}{\cPP}
$$
resulting in the supply/demand ratio for stored phosphorus,
$$
\sdP=\frac{\sPdw_6}{\dPdw} \in [0;1]
$$
On exit from step 6:

* The demand of the phosphorus store may not have been met, $\sdP\le1$
* $\sUP$ may have decreased.

### Roundup

The allocation in steps 1 to 6 served to compute the supplies and losses pertaining to the four compartments ($\wSdw,\wCdw,\wNdw,\wPdw$). We now apply these changes to update the model state variables:
$$
\begin{split}
\wSdw &\leftarrow \wSdw+\sSdw_1+\sSdw_3 \\[6pt]
\wCdw &\leftarrow \wCdw+\sCdw_1+\sCdw_2+\sCdw_3+\sCdw_4+\sCdw_5 \\[6pt]
\wNdw &\leftarrow \wNdw+\sNdw_2+\sNdw_3+\sNdw_4 \\[6pt]
\wPdw &\leftarrow \wPdw+\sPdw_2+\sPdw_6
\end{split}
$$
Note that any $\Delta$ is either $\le0$ or $\ge 0$; no $\Delta$ can attain both positive and negative values.

The total  building costs (eq. $\ref{eq_building_cost}$) are 
$$
\sBC = \lambdaS\sSdw_3 + \lambdaN\sNdw_4+ \lambdaC\sCdw_5
$$


## Calibration

While most model parameters can be estimated from biological principles or from empirical data, other parameters can only be estimated by comparison of simulation outputs with expected data This method of parameter estimation is known as 'calibration'. Calibration was carried out by trial-and-error and visual inspection of plots showing simulation outputs together with observations.

For the exudation costs (eq. $\ref{eq_exudation}$) we found

* $\lambda^C=0.02$ 
* $\lambda^N=0.02$  
* $\lambda^P=0.001$ 

For carbon supply, we set the scaling of leaf length to crown zone area (eq. $\ref{eq_crown_zone}$) to its maximum,

* $c_Z=2$

The uptake rates for nitrogen and phosphorus  (eq. $\ref{eq_NP_supply}$)  interacted much with the corresponding exudation rates. We arrived at

* $\beta=0.27$ 
* $\gamma=0.30$

This figure shows the driving variables  on the left $\big(I_d, [N_{water}], [P_{water}]\big)$, and simulated and observed plant concentrations on the right $\big(\cWC,\cWN,\cWP\big)$:

![](D:\Documents\sites\ecolmod3\media\models\saccharina\callibration-concentrations.png)

The upper brown lines show the upper limits on the respective concentrations. The model overshoots a bit because of the roughness of the time step (1 day). The two lower brown lines for $\cWN$ show the critical limits for its influence on plant photosynthesis. 

The  model is very sensitive to plant nitrogen $\cWN$ as seen in the simulated biomass $\wWdw$ and leaf area $A_l$:

![calibration-biomass-area](..\media\models\saccharina\calibration-biomass-area.png)

The dip in biomass at the of April is due to low plant nitrogen $\cWN$ concomitant with low light $I_d$.

## Verification

In model verification, we expose the model to different scenarios to check its robustness and to see if its outputs make sense.

### Darkness

In this scenario there was no sunlight, $I_d=0$. We expect the plant to exhaust itself by maintenance respiration and exudation. First, we check the biomass and leaf area:

![verification-darkness](..\media\models\saccharina\verification-darkness.png)

We see the expected exponential decline of the biomass, while the initial increase in leaf area is due to mobilisation of stores to build structural tissue. Yet, the initial, tiny increase looks suspicious. Let's study the details of the biomass compartments in the first few time steps:

```
      date structure.dw storeC.dw storeN.dw storeP.dw whole.dw    concC     concN      concP
2019/12/16        1.035     0.375     0.075     0.015      1.5   0.2624    0.0323     0.0033
2019/12/17      1.08675  0.223569  0.187547 0.0407837  1.53865 0.216615 0.0592255 0.00874703
2019/12/18      1.14109  0.105931  0.192444 0.0419632  1.48143 0.195363   0.06347 0.00934765
2019/12/19       1.1853       0.0  0.192444 0.0419632  1.41971 0.175327  0.066852 0.00975401
2019/12/20      1.15991       0.0  0.192444 0.0419632  1.39432 0.174696 0.0677051 0.00993162
```

That tiny increment turns out to be nitrogen! The nitrogen store was initialised at a value $\wSdw=0.075$ g dw/m that turned out to be out of balance with the nitrogen concentration in the water. As there are no upper limits to the rate of nutrient uptake, besides those set by the demands, the plant quickly made up for that imbalance and took up a large quantity of nitrogen in just one day. The needed building costs were taken from the carbon store.

Another issue ia that, in the longer run,  plant concentrations of nitrogen and phosphorus, drift out of control:![verification-darkness-conc](..\media\models\saccharina\verification-darkness-conc.png)

This happens because the model respires carbon from the structural tissue to meet the demand of basic metabolism (eq. $\ref{eq_maintenance_deficit}$), while leaving the stores alone. This needs to be fixed, even though it is not known exactly how the plant would react to extreme light limitation. A simple measure would be to carry out all steps of the allocation procedure, even when step 1 (maintenance respiration) leads to a carbon deficit. The model will also benefit from this by making it simpler: a carbon deficit is no longer considered an exceptional case.
