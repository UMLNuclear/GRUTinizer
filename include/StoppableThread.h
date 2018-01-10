#ifndef _STOPPABLETHREAD_H_
#define _STOPPABLETHREAD_H_

#ifndef __CINT__
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <thread>
#endif

#include <string>
#include <map>

#include "TObject.h"

class StoppableThread {
public:
  static void StopAll();
  static bool AnyThreadRunning();
  static std::string AnyThreadStatus();

  static void PauseAll();
  static void ResumeAll();

  StoppableThread(std::string name);
  static StoppableThread *Get(std::string name);
  static std::vector<StoppableThread*> GetAll();
  virtual ~StoppableThread();

  void Resume();
  void Pause();
  void Stop();
  bool IsPaused();
  bool IsRunning();
  void Join();

  virtual void OnEnd() { }
  virtual std::string Status();
  std::string Name() const     { return fname; }

  virtual void ClearQueue() { }

  //protected:
  virtual bool Iteration() = 0;

  virtual size_t GetItemsPopped()   { return -1; };
  virtual size_t GetItemsPushed()   { return -1; };
  virtual size_t GetItemsCurrent()  { return -1; };
  virtual size_t GetRate()          { return -1; };

  static int GetNThreads();

  static void Print();

#ifndef __CINT__
  static std::thread status_thread;
#endif
  static bool status_thread_on;
  static void start_status_thread();
  static void stop_status_thread();
  static void join_status_thread();
  static void status_out_loop();
  static void status_out();


protected:
  static std::map<std::string,StoppableThread*> fthreadmap;
#ifndef __CINT__
  static std::timed_mutex fthreadmap_mutex;
#endif

  //long items_in;
  //long items_out;
  //long rate;

private:
  StoppableThread(const StoppableThread& other) { }
  StoppableThread& operator=(const StoppableThread& other) { return *this; }

  std::string fname;

  void Loop();




#ifndef __CINT__
  std::thread thread;
  std::atomic_bool running;
  std::atomic_bool force_stop;
  std::atomic_bool paused;
  std::condition_variable paused_wait;
  std::mutex pause_mutex;
#endif

  ClassDef(StoppableThread, 0);
};


#endif /* _STOPPABLETHREAD_H_ */
