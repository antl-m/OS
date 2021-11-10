// Run() is called from Scheduling.main() and is where
// the scheduling algorithm written by the user resides.
// User modification should occur within the Run() function.

import java.util.Queue;
import java.util.Vector;
import java.io.*;

public class SchedulingAlgorithm {

  public static Results Run(int runtime, int quantumTime, Vector<sProcess> processVector, Results result) {
    int comptime = 0;
    int size = processVector.size();
    int completed = 0;
    String resultsFile = "Summary-Processes";

    result.schedulingType = "Preemptive";
    result.schedulingName = "Round-robin";
    try {
      //BufferedWriter out = new BufferedWriter(new FileWriter(resultsFile));
      //OutputStream out = new FileOutputStream(resultsFile);
      PrintStream out = new PrintStream(new FileOutputStream(resultsFile));
      Vector<sProcess> ReadyProcesses = new Vector<>();
      sProcess CurrentProcess = null;

      while (comptime < runtime && completed != size) {

        for (sProcess Process : processVector)
          if (Process.ArrivalTime == comptime)
            ReadyProcesses.add(Process);

        if (CurrentProcess == null && !ReadyProcesses.isEmpty())
        {
          CurrentProcess = ReadyProcesses.firstElement();
          out.println("Time point " + comptime + ": " + CurrentProcess.Register());
          ReadyProcesses.remove(CurrentProcess);
        }

        if (CurrentProcess != null && CurrentProcess.TimeSpent == CurrentProcess.ExecTime)
        {
          out.println("Time point " + comptime + ": " + CurrentProcess.Complete());
          completed++;
          CurrentProcess = null;
        }

        if (CurrentProcess != null && CurrentProcess.TimeSpentPerQuantum == quantumTime)
        {
          out.println("Time point " + comptime + ": " + CurrentProcess.Block());
          CurrentProcess.TimeSpentPerQuantum = 0;
          ReadyProcesses.add(CurrentProcess);
          CurrentProcess = null;
        }

        if (CurrentProcess != null)
        {
          CurrentProcess.TimeSpent++;
          CurrentProcess.TimeSpentPerQuantum++;
        }
        comptime++;
      }

      out.close();
    } catch (IOException e) { /* Handle exceptions */ }
    result.compuTime = comptime;
    return result;
  }
}
