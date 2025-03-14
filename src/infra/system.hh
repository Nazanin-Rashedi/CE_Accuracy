#ifndef INFRA_SYSTEM_HH
#define INFRA_SYSTEM_HH

#include "infra/glob_infra_standard_includes.hh"
#include <fstream>
#include <string>
#include <sys/utsname.h>
#include <unistd.h>
#include <stdlib.h>

#ifdef __linux__
 #include <sys/sysinfo.h>
 #include <endian.h>
#endif



#ifdef __x86_64
#include <cpuid.h>
#endif

// #include <libcpuid.h>


namespace GM {

class System {
  public:
    enum Vendor {
       kIntel = 0,
       kAmd   = 1,
       kArm   = 2,
       kNoVendor = 3
    };

    enum CacheType {
      kInstruction = 0,
      kData = 1,
      kUnified = 2,
      kNoCacheType = 3
    };

    struct cache_t {
      unsigned int _id;
      unsigned int _level;
      size_t       _size; // in B
      CacheType    _type;

      cache_t() : _id(0), _level(0), _size(0), _type(kNoCacheType) {}
      cache_t(const unsigned int aId, const unsigned int aLevel, const size_t aSize, const CacheType aType)
             : _id(aId), _level(aLevel), _size(aSize), _type(aType) {}

      inline unsigned int id() const { return _id; }
      inline unsigned int level() const { return _level; }
      inline unsigned int size() const { return _size; }
      inline unsigned int size_B() const { return _size; }
      inline unsigned int size_KB() const { return (_size / 1024); }
      inline CacheType    type() const { return _type; }

      std::ostream& print(std::ostream& os) const;
    };
    typedef std::vector<cache_t> cache_vt;

    typedef std::vector<unsigned int> thread_vt;
    struct core_t {
      unsigned int _coreid;
      thread_vt    _threads;
      core_t() : _coreid(0), _threads() {}
      core_t(unsigned int aCoreId) : _coreid(aCoreId), _threads() {}

      inline unsigned int coreid() const { return _coreid; }
      inline const thread_vt& threads() const { return _threads; }
      inline       thread_vt& threads() { return _threads; }

      std::ostream& print(std::ostream&);
    };
    typedef std::vector<core_t> core_vt;

    struct cpu_t {
      unsigned int _cpuid; // physical cpu
      double       _taktrate;
      std::string  _name;
      core_vt      _cores;
      cache_vt     _caches;

      cpu_t() : _cpuid(0), _taktrate(0), _name(), _cores(), _caches() {}
      cpu_t(unsigned int aCpuid) : _cpuid(aCpuid), _taktrate(0), _name(), _cores(), _caches() {}

      inline unsigned int cpuid() const { return _cpuid; }
      inline double       taktrate() const { return _taktrate; }
      inline const std::string& name() const { return _name; }
      inline const core_vt&  cores() const { return _cores; }
      inline       core_vt&  cores()       { return _cores; }
      inline const cache_vt& caches() const { return _caches; }
      inline       cache_vt& caches()       { return _caches; }

      inline const cache_t& cache(const size_t i) const { return _caches[i]; }
      inline const core_t&  core(const size_t i) const { return _cores[i]; }
    };

    typedef std::vector<cpu_t> cpu_vt;

  public:
    System(const int aHwThreadNo = 0);
  public:
    inline int   hwthreadno() const { return _hwthreadno; }
    inline bool  ok() const { return _ok; }
    inline const std::string& hostname() const { return _hostname; }
    inline const std::string& machine() const { return _machine; }
    inline const std::string& osname() const { return _osname; }
    inline const std::string& osrelease() const { return _osrelease; }
    inline const std::string& osversion() const { return _osversion; }
    inline const std::string& domainname() const { return _domainname; }

    inline Vendor vendor() const { return _vendor; }
    inline size_t no_cpus() const { return _cpus.size(); }
           size_t no_cores() const;
           size_t no_threads() const;
    inline const cpu_vt& cpus() const { return _cpus; }
    inline size_t clockticks() const { return _clockticks; }
    inline double cycleNS() const { return (((double) 1000L*1000L*1000L) / ((double) clockticks())); }
    inline size_t ram_total() const { return _ram_total; }
    inline size_t ram_free() const { return _ram_free; }
    inline size_t pagesize() const { return _pagesize; }

    inline size_t cachelinesize() const { return _cachelinesize; }

    inline const cpu_t& cpu(const size_t i) const { return _cpus[i]; }
  public:
    bool is_little_endian() const;
    bool is_big_endian() const;
  public:
    uint64_t get_clockticks(const uint aHwThreadNo) const; // get from cpufreq/scaling_cur_freq
    uint64_t get_ram_total() const;
    uint64_t get_ram_free() const;
    uint64_t get_pagesize() const;
    uint64_t get_cacheline_size() const;
  public:
    void print(std::ostream& os) const;
    void print_cpuid(std::ostream& os) const; // only for intel
  public:
    inline const uint_vt& sw2hwbinding() const { return _sw2hwbinding; }
  private:
    void extract_sw2hwbinding();
  private:
    void get_cpuid_info(); // only for intel
    void get_sys_devices_info();
    void get_sys_devices_info_linux();
    void get_sys_devices_info_mach();
  private:
    int         _hwthreadno;
    bool        _ok;
    std::string _hostname;
    std::string _osname;
    std::string _osversion;
    std::string _osrelease;
    std::string _machine;
    std::string _domainname;
    cpu_vt      _cpus;
    Vendor      _vendor;
    uint64_t    _clockticks; // as current
    size_t      _ram_total;
    size_t      _ram_free;
    size_t      _pagesize;
    size_t      _cachelinesize;
    uint_vt     _sw2hwbinding; // suggested binding of sw-threads to hw-threads
};


std::ostream&  operator<<(std::ostream&, const System::Vendor);
std::ostream&  operator<<(std::ostream&, const System::CacheType);

inline std::ostream& operator<<(std::ostream& os, const System::cache_t& x) { return x.print(os); }

} // end namespace


#endif
