public class sProcess {
  public int ProcessNum;
  public int ExecTime;
  public int ArrivalTime;
  public int TimeSpent;
  public int TimeSpentPerQuantum;
  public int BlockingCount;

  public sProcess (int _ProcessNum, int _ExecTime, int _ArrivalTime) {
    ProcessNum = _ProcessNum;
    ExecTime = _ExecTime;
    ArrivalTime = _ArrivalTime;
    TimeSpent = TimeSpentPerQuantum = BlockingCount = 0;
  }

  public String Register()
  {
    return "Registered: " + toString();
  }

  public String Complete()
  {
    return "Completed: " + toString();
  }

  public String Block()
  {
    BlockingCount++;
    return "Blocked: " + toString();
  }

  public String toString()
  {
    return "Process #" +    ProcessNum +
         "; ExecTime " +    ExecTime +
         "; ArrivalTime " + ArrivalTime +
         "; TimeSpent " +   TimeSpent +
         "; Blocked " +      BlockingCount+ ";";
  }
}
