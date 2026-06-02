#include "common_util.h"
#include "radix_tree_demo.h"

static void InitGlog(const char* argv0) {
  google::InitGoogleLogging(argv0);
  FLAGS_logtostderr = true;
  FLAGS_colorlogtostderr = true;
}

static void ReleaseGlog() { google::ShutdownGoogleLogging(); }

int main(int argc, char** argv) {
  UNUSED_PARAM(argc);
  InitGlog(argv[0]);
  radix_tree_demo::RadixTreeDemoRun();
  ReleaseGlog();
  return 0;
}
