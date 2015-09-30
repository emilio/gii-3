function dh = dhdt(t, h)
  A = 4;
  Q = 0.5;
  K = 0.2;
  dh = (1 / A) * (Q - K * sqrt(h))
end
