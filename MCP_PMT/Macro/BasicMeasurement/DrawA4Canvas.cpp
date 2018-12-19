void DrawA4Canvas(char *name="c1")
{
  TCanvas *canvas = (TCanvas*)gROOT->FindObject(name);
  if (canvas) delete canvas;
  const double w = 794;
  const double h = 1123;
  canvas = new TCanvas(name,name,w,h);
  canvas->SetWindowSize(w+(w-canvas->GetWw()),h+(h-canvas->GetWh()));
//  canvas->SetRightMargin(0.19);
  canvas->cd();
}
