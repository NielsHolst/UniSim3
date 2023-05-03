class BudgetLayer {
  BudgetLayer();
protected:
  struct {
    const double &a;
  } par, sw, lw;
  friend class BudgetSolver;
};


BudgetLayer()::BudgetLayer()
  :
  par.a(&swAbsorptivityTop),
  sw.a (&swAbsorptivityTop),
  lw.a (&lwAbsorptivityTop)
{
}