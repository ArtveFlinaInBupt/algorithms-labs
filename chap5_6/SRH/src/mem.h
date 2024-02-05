#pragma once

#ifndef MEM
#  define MEM

#  include <algorithm>
#  include <atomic>
#  include <future>
#  include <thread>

#  if defined(__APPLE__)
#    include <mach/mach.h>
#  elif defined(__linux__)
#    include <sys/resource.h>
#  else
#    error "Unsupported platform, only macOS and Linux are supported."
#  endif

inline unsigned long get_mem_usage() {
#  ifdef __APPLE__
  struct task_basic_info t_info;
  mach_msg_type_number_t t_info_count = TASK_BASIC_INFO_COUNT;
  return task_info(
             mach_task_self(), TASK_BASIC_INFO, (task_info_t)&t_info,
             &t_info_count
         ) == KERN_SUCCESS
             ? t_info.resident_size
             : 0;
#  else
  struct rusage r_usage;
  getrusage(RUSAGE_SELF, &r_usage);
  return r_usage.ru_maxrss * 1024;
#  endif
}

inline unsigned long monitor_mem_usage(std::atomic<bool> &keep_running) {
  unsigned long mem_usage = get_mem_usage();
  while (keep_running.load()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    mem_usage = std::max(mem_usage, get_mem_usage());
  }
  return mem_usage;
}

#endif // !MEM
