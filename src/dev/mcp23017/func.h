#if c_MCP23017 == 1
void PwdPompKick(int PwdChannel, int PwdMax, int PwdStart, int PwdNorm, int KickTime){
          ledcWrite(PwdChannel, PwdStart);
          delay(50);  
          ledcWrite(PwdChannel, PwdMax);  
          delay(KickTime);
          ledcWrite(PwdChannel, PwdNorm);
}
#endif // c_MCP23017