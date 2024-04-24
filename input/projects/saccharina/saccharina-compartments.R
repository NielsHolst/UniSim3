rm(list=ls(all=TRUE))

# Fixed parameters 
c_C_str   = 0.21
c_C_res_C = 0.47
c_C_res_N = 0.21
c_N_str   = 0.02
c_N_res_C = 0.00
c_N_res_N = 0.37

k = c_C_str*(c_N_res_C - c_N_res_N) + c_C_res_C*(c_N_res_N - c_N_str) + c_C_res_N*(c_N_str - c_N_res_C)
k

# Auxiliary variables
w1 = function(c_C_tot, c_N_tot) (c_C_res_C*(c_N_res_N - c_N_tot) + c_C_res_N*(c_N_tot - c_N_res_C) + c_C_tot*(c_N_res_C - c_N_res_N) )/k
w2 = function(c_C_tot, c_N_tot) (c_C_str*(c_N_tot - c_N_res_N) + c_C_res_N*(c_N_str - c_N_tot) + c_C_tot*(c_N_res_N - c_N_str)       )/k
w3 = function(c_C_tot, c_N_tot) (c_C_str*(c_N_res_C - c_N_tot) + c_C_res_C*(c_N_tot - c_N_str) + c_C_tot*(c_N_str - c_N_res_C)       )/k
w123  = function(c_C_tot, c_N_tot) {
  c(w1(c_C_tot, c_N_tot), w2(c_C_tot, c_N_tot), w3(c_C_tot, c_N_tot) )
}

# Question 1: Optimal biomass compartmentalisation

# Pre-conditions
w = w123(0.35, 0.05)
W_tot = 100
W_str   = w[1]*W_tot
W_res_C = w[2]*W_tot
W_res_N = w[3]*W_tot

# Compartments
W_str
W_res_C
W_res_N
W_str + W_res_C + W_res_N

# Question 2: Reserve demands

# Pre-conditions
w = w123(0.35, 0.05)
W_str   = 34.50
W_res_C = 40
W_res_N = 3
DW_str  = 4
total = W_str + W_res_C + W_res_N + DW_str

# Proportions
(W_str+DW_str)/total
W_res_C/total
W_res_N/total

# Demands
DW_res_C = w[2]/w[1]*(W_str + DW_str) - W_res_C
DW_res_N = w[3]/w[1]*(W_str + DW_str) - W_res_N
new_W_tot = (W_str + DW_str)/w[1]

DW_res_C
DW_res_N
new_W_tot
W_str + DW_str + W_res_C + DW_res_C + W_res_N + DW_res_N

# Quick check
(W_str+DW_str)/new_W_tot
(W_res_C+DW_res_C)/new_W_tot
(W_res_N+DW_res_N)/new_W_tot
