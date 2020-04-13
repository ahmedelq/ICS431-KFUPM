import copy
import statistics

class Process:
    def __init__(self, name, arrival_time, execution_time, inturrpt_time, IOTime, priority):
        self.name = name
        self.arrival_time = arrival_time
        self.cpu_time = execution_time
        self.inturrpt_time = inturrpt_time
        self.IO_time = IOTime
        self.priority = priority
        self.tt = 0
        self.w = 0
    def __str__(self):
        return self.name
    def __repr__(self):
        return "<{}>".format(self.name)


def read_proc():
    file_path = input("The name of the file to be read:\t")
    ps = open(file_path).read()
    res = []
    for proc in ps.split("\n"):
        p = proc.split(" ")
        p = list(filter(len, p))
        if p:
            p = p[:1] + list(map(float, p[1:]))
            res.append( Process(p[0], p[1], p[2], p[3], p[4], p[5]) )
    return res
        
def sch_print(jobs, finished):
    print("#\tWaiting\tt(CPU)\tt(all)")
    jobs.sort(key=lambda p: p.name)
    for p in jobs:
        print(p, p.w, p.tt, p.tt+p.inturrpt_time+p.IO_time, sep="\t")

    print("\nAvg. cpu turnaround:", sum(p.tt for p in jobs)/len(jobs))
    print("Avg. total turnaround:", sum(p.tt+p.inturrpt_time+p.IO_time for p in jobs)/len(jobs))
    print("Stdv. cpu turnaround:", statistics.stdev(p.tt for p in jobs) )
    print("Stdv. total turnaround:", statistics.stdev(p.tt+p.inturrpt_time+p.IO_time for p in jobs) )
    print("Avg. wait:", sum(p.w for p in jobs) /len(jobs) )
    print("Stdv. wait:", statistics.stdev(p.w for p in jobs) )
    print("\nScheduling Order:")
    print(" ".join(p[0].name for p in finished))
    print("\n\n")

def fcfs(res):
    fcfs = [copy.deepcopy(p) for p in res]
    fcfs = sorted(fcfs, key=lambda p: p.arrival_time)
    fcfs_finished = []
    ft = 0
    for proc in fcfs:
        proc.w += ft - proc.arrival_time #waiting time
        ft = ft + proc.cpu_time #finish from cpu
        fcfs_finished.append( (proc, ft) )
        proc.tt += ft - proc.arrival_time
    print("\n---- FCFS ----\n")
    sch_print(fcfs, fcfs_finished)

def sch_job(jobs, job, ct):
    if jobs and jobs[-1][0] == job: 
        jobs[-1] = (job, ct)
    else:
        jobs.append( (job, ct) )
        
def inc_wt(jobs, current_proc, time_amt, ct):
    for p in jobs:
        if p != current_proc and p.arrival_time <= ct and p.cpu_time:
            p.w += time_amt
            
def sjf(res):
    sjf = [copy.deepcopy(p) for p in res]
    sjf = sorted(sjf, key=lambda p: (p.arrival_time, p.cpu_time))
    sjf_arrv = sorted([p.arrival_time for p in sjf])
    sjf_arrv.remove(0)

    ct = 0
    proc = sjf[0]
    finished = []
    while(sjf_arrv):
        nat = sjf_arrv.pop(0)
        sch_job(finished, proc, ct)
        if(proc.cpu_time < (nat-ct) ):
            sjf_arrv = [nat] + sjf_arrv
            inc_wt(sjf, proc, proc.cpu_time, ct) 
            
            ct += proc.cpu_time
            proc.cpu_time = 0
            proc.tt = ct - proc.arrival_time
        else:
            inc_wt(sjf, proc, (nat - ct), ct)
            proc.cpu_time -= nat-ct
            ct = nat
        proc = min(filter(lambda p: p.arrival_time <= ct and p.cpu_time !=0, sjf), key=lambda p:p.cpu_time)


    sjf = sorted(sjf, key=lambda p: p.cpu_time)
    for proc in sjf:
        if not proc.cpu_time:continue
        
        sch_job(finished, proc, ct)
        ct += proc.cpu_time
        for p in sjf:
            if p.cpu_time and p!=proc:
                p.w += proc.cpu_time
        
        proc.tt = ct - proc.arrival_time 
        proc.cpu_time = 0
    print("\n---- Preemptive SJF ----\n")
    sch_print(sjf, finished)

def inc_rr_wt(jobs, wt):
    #print(jobs, wt)
    for p in jobs:
        p.w += wt


def rr_sch(jobs, quantum):
    rr_jobs = [copy.deepcopy(p) for p in jobs]
    rr_jobs.sort(key=lambda p: (p.arrival_time, p.priority) )
    ct = 0
    finished = []
    proc_queue = [ rr_jobs[0] ]
    while( proc_queue ):
        
        np = proc_queue.pop(0)
        sch_job(finished, np, ct)
        
        time_slice = min(quantum, np.cpu_time) #quantum/remaining cpu_time 
        inc_rr_wt(proc_queue, time_slice)
        ct += time_slice
        np.cpu_time -= time_slice
        
        
        for p in rr_jobs:
            if p != np and p.cpu_time and p.arrival_time <= ct and p not in proc_queue:
                p.w += ct - p.arrival_time
                proc_queue.append(p)
                
        
        if np.cpu_time:
            proc_queue.append(np)
        else:
            np.tt = ct - np.arrival_time
    
    print("\n---- RR ----")
    print("Quantum time:", quantum, "\n")
    sch_print(rr_jobs, finished)

if __name__ == "__main__":
    read_processes = read_proc()
    fcfs(read_processes)
    sjf(read_processes)
    rr_sch(read_processes, 3)
    
    