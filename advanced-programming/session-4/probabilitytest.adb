with Text_IO;

procedure ProbabilityTest is
  subtype Probability is Float digits 2 range 0.0..1.0;
  package Probability_IO is new Text_IO.Float_IO(Probability);
  prob: Probability;
begin
  prob := 0.043;
  Probability_IO.Put(prob);
end ProbabilityTest;
