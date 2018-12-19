void DrawSquareCanvas(char *name="c1")
{
  TCanvas *canvas = (TCanvas*)gROOT->FindObject(name);
  if (canvas) delete canvas;
  canvas = new TCanvas(name,name,540,502);
  canvas->SetRightMargin(0.19);
  canvas->cd();
}
