class MickeyMouse
{
  private:
    float g_fpadRow;
    float g_fpadCol;
    float g_fpadRowDir;
    float g_fpadColDir;
    int g_nrows;
    int g_ncols;
    MousePadData* g_p_mousePadData;
    CubeData* g_p_cubeData;

  public:
    MickeyMouse(MousePadData* p_mousePadData, CubeData* p_cubeData, int nrows, int ncols);
    void zero();
    void ramp();
    void flat();
    void randomWalk();
    void test();
};
MickeyMouse::MickeyMouse(MousePadData* p_mousePadData, CubeData* p_cubeData, int nrows, int ncols)
{
    g_fpadRow = 0.0;
    g_fpadCol = 0.0;
    g_fpadRowDir = 0.001 *(float) random(1000);
    g_fpadColDir = 1.0 - sqrt(g_fpadRowDir * g_fpadRowDir);
    g_p_mousePadData = p_mousePadData;
    g_p_cubeData = p_cubeData;
    g_nrows = nrows;
    g_ncols = ncols;
}
void MickeyMouse::randomWalk()
{
  g_fpadRow = g_fpadRow + 0.01 * ((float) g_p_cubeData->mouseStep) * g_fpadRowDir;
  if (g_fpadRow < 0.0)
  {
    g_fpadRow = 0.0;
    g_fpadRowDir = 0.001 *(float) random(1000);
    if (g_fpadColDir >= 0)
    {
      g_fpadColDir = 1.0 - sqrt(g_fpadRowDir * g_fpadRowDir);
    }
    else
    {
       g_fpadColDir = sqrt(g_fpadRowDir * g_fpadRowDir) - 1.0;
    }
  }
  float ftop = g_fpadRow + 0.01 * ((float) g_p_cubeData->mouseSize);
  if (ftop >= (float) g_nrows)
  {
    ftop = (float) g_nrows - 0.01;
    g_fpadRow = ftop - 0.01 * ((float) g_p_cubeData->mouseSize);
    g_fpadRowDir = -0.001 *(float) random(1000);
    if (g_fpadColDir >= 0)
    {
      g_fpadColDir = 1.0 - sqrt(g_fpadRowDir * g_fpadRowDir);
    }
    else
    {
       g_fpadColDir = sqrt(g_fpadRowDir * g_fpadRowDir) - 1.0;
    }
  }
  g_fpadCol = g_fpadCol + 0.01 * ((float) g_p_cubeData->mouseStep) * g_fpadColDir;
  if (g_fpadCol < 0.0)
  {
    g_fpadCol = 0.0;
    g_fpadColDir = 0.001 *(float) random(1000);
    if (g_fpadRowDir >= 0)
    {
      g_fpadRowDir = 1.0 - sqrt(g_fpadColDir * g_fpadColDir);
    }
    else
    {
       g_fpadRowDir = sqrt(g_fpadColDir * g_fpadColDir) - 1.0;
    }
  }
  float fright = g_fpadCol + 0.01 * ((float) g_p_cubeData->mouseSize);
  if (fright >= (float) g_ncols)
  {
    fright = (float) g_ncols - 0.01;
    g_fpadCol = fright - 0.01 * ((float) g_p_cubeData->mouseSize);
    g_fpadColDir = -0.001 *(float) random(1000);
    if (g_fpadRowDir >= 0)
    {
      g_fpadRowDir = 1.0 - sqrt(g_fpadColDir * g_fpadColDir);
    }
    else
    {
       g_fpadRowDir = sqrt(g_fpadColDir * g_fpadColDir) - 1.0;
    }
  }
  this->zero();
  float frowStart = g_fpadRow;
  float frowEnd   = ftop;
  float fcolStart = g_fpadCol;
  float fcolEnd   = fright;

  int irowStart = (int) frowStart;
  int irowEnd   = (int) frowEnd;
  int icolStart = (int) fcolStart;
  int icolEnd   = (int) fcolEnd;

  float fdensity = 10000.0 * (float) g_p_cubeData->mouseWeight / (((float) g_p_cubeData->mouseSize) * ((float) g_p_cubeData->mouseSize));
  for (int irow = irowStart; irow <= irowEnd; ++ irow)
  {
    float frowFrac = 0.0;
    if ((irowStart <  irow) && (irow <  irowEnd)) frowFrac = 1.0;
    if ((irowStart == irow) && (irow <  irowEnd)) frowFrac = (float) (irow + 1) - frowStart;
    if ((irowStart <  irow) && (irow == irowEnd)) frowFrac = frowEnd - (float) irow;
    if ((irowStart == irow) && (irow == irowEnd)) frowFrac = frowEnd - frowStart;
    for (int icol = icolStart; icol <= icolEnd; ++icol)
    {
      float fcolFrac = 0.0;
      if ((icolStart <  icol) && (icol <  icolEnd)) fcolFrac = 1.0;
      if ((icolStart == icol) && (icol <  icolEnd)) fcolFrac = (float) (icol + 1) - fcolStart;
      if ((icolStart <  icol) && (icol == icolEnd)) fcolFrac = fcolEnd - (float) icol;
      if ((icolStart == icol) && (icol == icolEnd)) fcolFrac = fcolEnd - fcolStart;
      float fweight = fdensity * frowFrac * fcolFrac;
      g_p_mousePadData->pad[irow][icol] = (int16_t) fweight;
      
    }
  }
/*
  Serial.print(ftop);
  Serial.print(",");
  Serial.print(g_fpadRow);
  Serial.print(",");
  Serial.print(fright);
  Serial.print(",");
  Serial.println(g_fpadCol);
*/
//  this->test();
}
void MickeyMouse::test()
{
  float favgRow = 0.0;
  float favgCol = 0.0;
  float fweight = 0.0;
  for (int ii = 0; ii < g_nrows; ++ii)
  {
    for (int ij = 0; ij < g_ncols; ++ij)
    {
      fweight = fweight + ((float) g_p_mousePadData->pad[ii][ij]);
      favgRow = favgRow + ((float) g_p_mousePadData->pad[ii][ij]) * ((float) ii);
      favgCol = favgCol + ((float) g_p_mousePadData->pad[ii][ij]) * ((float) ij);
    }
  }
  favgRow = favgRow / fweight;
  favgCol = favgCol / fweight;
//  Serial.println(fweight);
//  Serial.print(favgRow);
//  Serial.print(",");
//  Serial.println(favgCol);  
}
void MickeyMouse::zero()
{
  for (int ii = 0; ii < g_nrows; ++ii)
  {
    for (int ij = 0; ij < g_ncols; ++ij)
    {
      g_p_mousePadData->pad[ii][ij] = 0;
    }
  }
}
void MickeyMouse::ramp()
{
  for (int ii = 0; ii < g_nrows; ++ii)
  {
    for (int ij = 0; ij < g_ncols; ++ij)
    {
      g_p_mousePadData->pad[ii][ij] = ij + ii * g_ncols;
    }
  }
}
void MickeyMouse::flat()
{
  for (int ii = 0; ii < g_nrows; ++ii)
  {
    for (int ij = 0; ij < g_ncols; ++ij)
    {
      g_p_mousePadData->pad[ii][ij] =  g_p_cubeData->mouseWeight;
    }
  }
}
