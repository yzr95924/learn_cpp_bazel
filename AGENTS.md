# AGENTS.md

## Build & Test Commands

```bash
# Full clean build (ASan, debug)
bash scripts/build.sh

# Incremental rebuild (no clean)
bash scripts/rebuild.sh

# Run all unit tests
bash scripts/run_ut.sh

# Build single target
bazel build --compilation_mode=dbg --define=use_asan=true //src/app:main

# Test single UT
bazel test --compilation_mode=dbg --define=use_asan=true @learn_c_bazel//ut:ut_main --test_output=all

# Termux (Android) — use use_termux instead of use_asan
bazel test --compilation_mode=dbg --define=use_termux=true @learn_c_bazel//ut:ut_main --test_output=all
```

- Scripts use `@learn_c_bazel//src/...` (canonical repo name); `//src/...` also works in-workspace.
- All bazel invocations pass `--compilation_mode=dbg --define=use_asan=true` (or `use_termux=true`).
- Scripts add `--subcommands=pretty_print --verbose_failures` for debugging build failures.
- Regenerate `compile_commands.json` after build (requires `bazel-compile-commands` **and** `jq` on PATH):
  `python3 scripts/generate_compile_commands.py -o "--compilation_mode=dbg"`

## Architecture

- **Language**: C++17, compiled with `clang++`/`lld` (set in `.bazelrc`); `--jobs=8`.
- **Build system**: Bazel with bzlmod (`MODULE.bazel`), legacy `WORKSPACE` (`name = "learn_c_bazel"`) also present.
- **Local build framework**: `bazel_buildframe/` is a local Bazel module providing `copts.bzl` (strict warnings, ASan flags, debug/release link opts) and `pkg_config.bzl`.
- **Source**: `src/` — libraries under subdirectories; app entrypoint at `//src/app:main`.
- **Shared header**: `//src/include:inner_include` provides `common_util.h` (defines `UNUSED_PARAM`, `RETURN_INNER_OK`, `RETURN_INNER_ERR`).
- **Tests**: Each `ut/*_ut/` is a `cc_library` with `alwayslink = True`; all are linked into a single `cc_test` target `//ut:ut_main`.
- **Test framework**: Google Test + Google Logging (glog init in `ut/ut_main.cc`); test data at `//data:test_data`.

## BUILD.bazel Conventions

- Always load from `@local_bazel_buildframe//:copts.bzl`: `GLOBAL_DEFAULT_COPTS`, `GLOBAL_DEFAULT_LINK_OPTS`.
- Always load rules from `@rules_cc//cc:defs.bzl`.
- Dependency naming: `@com_glog`, `@com_googletest`, `@com_google_benchmark`, `@com_cjson`, `@com_liburing`, `@com_google_protobuf`, `@local_libpq` (system libpq via pkg-config), `@local_bazel_buildframe`.
- Config settings in root `BUILD.bazel`: `//:debug_mode`, `//:release_mode`, `//:use_asan`, `//:use_termux`, `//:linux_x86_64`.
- Warnings as errors (`-Werror`) including `-Wconversion`, `-Wsign-conversion`, `-Wpedantic` (defined in `copts.bzl`).
- Protobuf targets: load `proto_library` from `@rules_proto//proto:defs.bzl`, `cc_proto_library` from `@com_google_protobuf//bazel:cc_proto_library.bzl`. Add warning suppressions to any target that includes protobuf/abseil headers:
  `-Wno-gnu-anonymous-struct -Wno-gnu-zero-variadic-macro-arguments -Wno-nested-anon-types -Wno-gcc-compat`

## Code Style

- All code follows Google C++ Style (classes, namespaces, std:: containers).
- `.clang-format`: Google-based, 4-space indent, 100-column limit, right-aligned pointers (`*p` not `* p`).
- `.pylintrc`: Google Python style, max line length 80.
