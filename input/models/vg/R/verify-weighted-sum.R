
M = data.frame(
  Material = c("A","B","C", "Combined"),
  a = c(0.5, 0.4, 0.3, 0),
  r = c(0.2, 0.1, 0.3, 0),
  t = c(0.3, 0.5, 0.4, 0)
)

w = c(0.1, 0.3, 0.6)

M$ua = w*M$a
M$ur = w*M$r
M$ut = w*M$t
M

M$ua[4] = sum(M$ua[1:3])
M$ur[4] = sum(M$ur[1:3])
M$ut[4] = sum(M$ut[1:3])
M
