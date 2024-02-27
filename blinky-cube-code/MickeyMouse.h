class MickeyMouse
{
  private:
    int g_ipadRow;
    int g_ipadCol;
    int g_ipadRowDir;
    int g_ipadColDir;
    int g_nrows;
    int g_ncols;
    MousePadData* g_p_mousePadData;
    CubeData* g_p_cubeData;

  public:
    MickeyMouse(MousePadData* p_mousePadData, CubeData* p_cubeData, int nrows, int ncols);
    void backNforth();
    void zero();
    void ramp();
    void flat();
};
MickeyMouse::MickeyMouse(MousePadData* p_mousePadData, CubeData* p_cubeData, int nrows, int ncols)
{
    g_ipadRow = 0;
    g_ipadCol = 0;
    g_ipadRowDir = 1;
    g_ipadColDir = 1;
    g_p_mousePadData = p_mousePadData;
    g_p_cubeData = p_cubeData;
    g_nrows = nrows;
    g_ncols = ncols;
}
void MickeyMouse::backNforth()
{
  g_p_mousePadData->pad[g_ipadRow][g_ipadCol] = 0;
  g_ipadCol = g_ipadCol + g_ipadColDir;
  if (g_ipadCol >= g_ncols)
  {
    g_ipadCol = g_ncols - 1;
    g_ipadColDir = -1;
    g_ipadRow = g_ipadRow + g_ipadRowDir;
    if (g_ipadRow >= g_nrows)
    {
      g_ipadRow = g_nrows - 1;
      g_ipadRowDir = -1;
    }
    if (g_ipadRow < 0)
    {
      g_ipadRow = 0;
      g_ipadRowDir = 1;
    }
  }
  if (g_ipadCol < 0)
  {
    g_ipadCol = 0;
    g_ipadColDir = 1;
    g_ipadRow = g_ipadRow + g_ipadRowDir;
    if (g_ipadRow >= g_nrows)
    {
      g_ipadRow = g_nrows - 1;
      g_ipadRowDir = -1;
    }
    if (g_ipadRow < 0)
    {
      g_ipadRow = 0;
      g_ipadRowDir = 1;
    }
  }
  g_p_mousePadData->pad[g_ipadRow][g_ipadCol] = g_p_cubeData->mouseWeight;
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
