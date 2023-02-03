# Question 1

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
w1 = function(c_C_tot, c_N_tot, W_tot) c_C_res_C*(c_N_res_N - c_N_tot) + c_C_res_N*(c_N_tot - c_N_res_C) + c_C_tot*(c_N_res_C - c_N_res_N)
w2 = function(c_C_tot, c_N_tot, W_tot) c_C_str*(c_N_tot - c_N_res_N) + c_C_res_N*(c_N_str - c_N_tot) + c_C_tot*(c_N_res_N - c_N_str)
w3 = function(c_C_tot, c_N_tot, W_tot) c_C_str*(c_N_res_C - c_N_tot) + c_C_res_C*(c_N_tot - c_N_str) + c_C_tot*(c_N_str - c_N_res_C)
w123  = function(c_C_tot, c_N_tot, W_tot) {
  c(
    w1(c_C_tot, c_N_tot, W_tot),
    w2(c_C_tot, c_N_tot, W_tot),
    w3(c_C_tot, c_N_tot, W_tot)
  )
}

w = w123(0.35, 0.05, 100)
W_str   = w[1]/k*W_tot
W_res_C = w[2]/k*W_tot
W_res_N = w[3]/k*W_tot

W_str
W_res_C
W_res_N

W_str + W_res_C + W_res_N

# Question 2

rm(list=ls(all=TRUE))
 
c_C_str   = 0.21
c_C_res_C = 0.47
c_C_res_N = 0.21
c_N_str   = 0.02
c_N_res_C = 0
c_N_res_N = 0.37

c_C_tot = 0.35
c_N_tot = 0.05

k = c_C_str*(c_N_res_C - c_N_res_N) + c_C_res_C*(c_N_res_N - c_N_str) + c_C_res_N*(c_N_str - c_N_res_C)
k

W_str   = 34.50
W_res_C = 40
W_res_N = 3
DW_str  = 4

w1 = c_C_res_C*(c_N_res_N - c_N_tot) + c_C_res_N*(c_N_tot - c_N_res_C) + c_C_tot*(c_N_res_C - c_N_res_N)
w2 = c_C_str*(c_N_tot - c_N_res_N) + c_C_res_N*(c_N_str - c_N_tot) + c_C_tot*(c_N_res_N - c_N_str)
w3 = c_C_str*(c_N_res_C - c_N_tot) + c_C_res_C*(c_N_tot - c_N_str) + c_C_tot*(c_N_str - c_N_res_C)

OW_str   = W_str + DW_str
OW_tot   = k/w1*OW_str
OW_res_C = w2/k*OW_tot
OW_res_N = w3/k*OW_tot

OW_tot
OW_res_C
OW_res_N
OW_str + OW_res_C + OW_res_N

DW_res_C = OW_res_C - W_res_C
DW_res_N = OW_res_N - W_res_N

DW_res_C
DW_res_N


