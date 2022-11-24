//Шаг одним насосом вперед
void StepOneDrvForward(int A, int B, int C, int D, int del)
{
    // enn
    bitWrite(bitw, A, 0);
    bitWrite(bitw, B, 1);
    bitWrite(bitw, C, 1);
    bitWrite(bitw, D, 0);
    mcp.writeGPIOAB(bitw);
    delayMicroseconds(del);
    // twee
    bitWrite(bitw, A, 0);
    bitWrite(bitw, B, 1);
    bitWrite(bitw, C, 0);
    bitWrite(bitw, D, 1);
    mcp.writeGPIOAB(bitw);
    delayMicroseconds(del);
    // drie
    bitWrite(bitw, A, 1);
    bitWrite(bitw, B, 0);
    bitWrite(bitw, C, 0);
    bitWrite(bitw, D, 1);
    mcp.writeGPIOAB(bitw);
    delayMicroseconds(del);
    // vier
    bitWrite(bitw, A, 1);
    bitWrite(bitw, B, 0);
    bitWrite(bitw, C, 1);
    bitWrite(bitw, D, 0);
    mcp.writeGPIOAB(bitw);
    delayMicroseconds(del);
}

//Шаг одним насосом назад
void StepOneDrvBackward(int A, int B, int C, int D, int del)
{
    // enn
    bitWrite(bitw, A, 0);
    bitWrite(bitw, B, 1);
    bitWrite(bitw, C, 1);
    bitWrite(bitw, D, 0);
    mcp.writeGPIOAB(bitw);
    delayMicroseconds(del);
    // twee
    bitWrite(bitw, A, 1);
    bitWrite(bitw, B, 0);
    bitWrite(bitw, C, 1);
    bitWrite(bitw, D, 0);
    mcp.writeGPIOAB(bitw);
    delayMicroseconds(del);
    // drie
    bitWrite(bitw, A, 1);
    bitWrite(bitw, B, 0);
    bitWrite(bitw, C, 0);
    bitWrite(bitw, D, 1);
    mcp.writeGPIOAB(bitw);
    delayMicroseconds(del);
    // vier
    bitWrite(bitw, A, 0);
    bitWrite(bitw, B, 1);
    bitWrite(bitw, C, 0);
    bitWrite(bitw, D, 1);
    mcp.writeGPIOAB(bitw);
    delayMicroseconds(del);
}

//Шаг двумя насосами вперед
void StepTwoDrvForward(int AA, int AB, int AC, int AD, int BA, int BB, int BC, int BD, int del)
{
    del = preferences.getInt("StPumpA_Del", 700);
    ret = preferences.getInt("StPumpA_Ret", 700);
    // enn
    bitWrite(bitw, AA, 0);
    bitWrite(bitw, AB, 1);
    bitWrite(bitw, AC, 1);
    bitWrite(bitw, AD, 0);
    bitWrite(bitw, BA, 0);
    bitWrite(bitw, BB, 1);
    bitWrite(bitw, BC, 1);
    bitWrite(bitw, BD, 0);
    mcp.writeGPIOAB(bitw);
    delayMicroseconds(ret);
    bitWrite(bitw, AA, 0);
    bitWrite(bitw, AB, 0);
    bitWrite(bitw, AC, 0);
    bitWrite(bitw, AD, 0);
    bitWrite(bitw, BA, 0);
    bitWrite(bitw, BB, 0);
    bitWrite(bitw, BC, 0);
    bitWrite(bitw, BD, 0);
    mcp.writeGPIOAB(bitw);
    delayMicroseconds(del);
    // twee
    bitWrite(bitw, AA, 0);
    bitWrite(bitw, AB, 1);
    bitWrite(bitw, AC, 0);
    bitWrite(bitw, AD, 1);
    bitWrite(bitw, BA, 0);
    bitWrite(bitw, BB, 1);
    bitWrite(bitw, BC, 0);
    bitWrite(bitw, BD, 1);
    mcp.writeGPIOAB(bitw);
    delayMicroseconds(ret);
    bitWrite(bitw, AA, 0);
    bitWrite(bitw, AB, 0);
    bitWrite(bitw, AC, 0);
    bitWrite(bitw, AD, 0);
    bitWrite(bitw, BA, 0);
    bitWrite(bitw, BB, 0);
    bitWrite(bitw, BC, 0);
    bitWrite(bitw, BD, 0);
    mcp.writeGPIOAB(bitw);
    delayMicroseconds(del);
    // drie
    bitWrite(bitw, AA, 1);
    bitWrite(bitw, AB, 0);
    bitWrite(bitw, AC, 0);
    bitWrite(bitw, AD, 1);
    bitWrite(bitw, BA, 1);
    bitWrite(bitw, BB, 0);
    bitWrite(bitw, BC, 0);
    bitWrite(bitw, BD, 1);
    mcp.writeGPIOAB(bitw);
    delayMicroseconds(ret);
    bitWrite(bitw, AA, 0);
    bitWrite(bitw, AB, 0);
    bitWrite(bitw, AC, 0);
    bitWrite(bitw, AD, 0);
    bitWrite(bitw, BA, 0);
    bitWrite(bitw, BB, 0);
    bitWrite(bitw, BC, 0);
    bitWrite(bitw, BD, 0);
    mcp.writeGPIOAB(bitw);
    delayMicroseconds(del);
    // vier
    bitWrite(bitw, AA, 1);
    bitWrite(bitw, AB, 0);
    bitWrite(bitw, AC, 1);
    bitWrite(bitw, AD, 0);
    bitWrite(bitw, BA, 1);
    bitWrite(bitw, BB, 0);
    bitWrite(bitw, BC, 1);
    bitWrite(bitw, BD, 0);
    mcp.writeGPIOAB(bitw);
    delayMicroseconds(ret);
    bitWrite(bitw, AA, 0);
    bitWrite(bitw, AB, 0);
    bitWrite(bitw, AC, 0);
    bitWrite(bitw, AD, 0);
    bitWrite(bitw, BA, 0);
    bitWrite(bitw, BB, 0);
    bitWrite(bitw, BC, 0);
    bitWrite(bitw, BD, 0);
    mcp.writeGPIOAB(bitw);
    delayMicroseconds(del);
}

//Шаг двумя насосами назад
void StepTwoDrvBackward(int AA, int AB, int AC, int AD, int BA, int BB, int BC, int BD, int del)
{
    // enn
    bitWrite(bitw, AA, 0);
    bitWrite(bitw, AB, 1);
    bitWrite(bitw, AC, 1);
    bitWrite(bitw, AD, 0);
    bitWrite(bitw, BA, 0);
    bitWrite(bitw, BB, 1);
    bitWrite(bitw, BC, 1);
    bitWrite(bitw, BD, 0);
    mcp.writeGPIOAB(bitw);
    delayMicroseconds(del);
    bitWrite(bitw, AA, 0);
    bitWrite(bitw, AB, 0);
    bitWrite(bitw, AC, 0);
    bitWrite(bitw, AD, 0);
    bitWrite(bitw, BA, 0);
    bitWrite(bitw, BB, 0);
    bitWrite(bitw, BC, 0);
    bitWrite(bitw, BD, 0);
    mcp.writeGPIOAB(bitw);
    delayMicroseconds(del);
    // twee
    bitWrite(bitw, AA, 1);
    bitWrite(bitw, AB, 0);
    bitWrite(bitw, AC, 1);
    bitWrite(bitw, AD, 0);
    bitWrite(bitw, BA, 1);
    bitWrite(bitw, BB, 0);
    bitWrite(bitw, BC, 1);
    bitWrite(bitw, BD, 0);
    mcp.writeGPIOAB(bitw);
    delayMicroseconds(del);
    bitWrite(bitw, AA, 0);
    bitWrite(bitw, AB, 0);
    bitWrite(bitw, AC, 0);
    bitWrite(bitw, AD, 0);
    bitWrite(bitw, BA, 0);
    bitWrite(bitw, BB, 0);
    bitWrite(bitw, BC, 0);
    bitWrite(bitw, BD, 0);
    mcp.writeGPIOAB(bitw);
    delayMicroseconds(del);
    // drie
    bitWrite(bitw, AA, 1);
    bitWrite(bitw, AB, 0);
    bitWrite(bitw, AC, 0);
    bitWrite(bitw, AD, 1);
    bitWrite(bitw, BA, 1);
    bitWrite(bitw, BB, 0);
    bitWrite(bitw, BC, 0);
    bitWrite(bitw, BD, 1);
    mcp.writeGPIOAB(bitw);
    delayMicroseconds(del);
    bitWrite(bitw, AA, 0);
    bitWrite(bitw, AB, 0);
    bitWrite(bitw, AC, 0);
    bitWrite(bitw, AD, 0);
    bitWrite(bitw, BA, 0);
    bitWrite(bitw, BB, 0);
    bitWrite(bitw, BC, 0);
    bitWrite(bitw, BD, 0);
    mcp.writeGPIOAB(bitw);
    delayMicroseconds(del);
    // vier
    bitWrite(bitw, AA, 0);
    bitWrite(bitw, AB, 1);
    bitWrite(bitw, AC, 0);
    bitWrite(bitw, AD, 1);
    bitWrite(bitw, BA, 0);
    bitWrite(bitw, BB, 1);
    bitWrite(bitw, BC, 0);
    bitWrite(bitw, BD, 1);
    mcp.writeGPIOAB(bitw);
    delayMicroseconds(del);
    bitWrite(bitw, AA, 0);
    bitWrite(bitw, AB, 0);
    bitWrite(bitw, AC, 0);
    bitWrite(bitw, AD, 0);
    bitWrite(bitw, BA, 0);
    bitWrite(bitw, BB, 0);
    bitWrite(bitw, BC, 0);
    bitWrite(bitw, BD, 0);
    mcp.writeGPIOAB(bitw);
    delayMicroseconds(del);
}
