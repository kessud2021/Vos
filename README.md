# VSS-CO OS

**A production-grade, Linux-like operating system for the 2020s**

![Status](https://img.shields.io/badge/status-active%20development-blue)
![License](https://img.shields.io/badge/license-MIT-green)
![C/C++/Go](https://img.shields.io/badge/languages-C%2FC%2B%2B%2FGo-red)

## Vision

VSS-CO OS is a **free, open-source, production-grade operating system** designed for:

- **Multiuser & Multitasking**: POSIX-compliant process management
- **Multi-platform**: x86-64, ARM64, RISC-V, PowerPC
- **Enterprise Security**: MAC (Mandatory Access Control), capability-based access control
- **High Performance**: Low-latency kernel, efficient scheduler, minimal overhead
- **Customization**: Modular design, buildable for embedded → servers → mobile
- **Portability**: From IoT devices to Android-compatible mobile systems

## Architecture

```
VSS-CO OS
├── Kernel (C + ASM)          - Core OS, hardware abstraction
├── Shell (C)                 - GNU Bash-compatible
├── System Utils (C/C++)      - ls, grep, tar, etc.
├── GUI (C++)                 - Desktop environment
├── Package Manager (Go)      - Binary & source package management
└── Libraries (C/C++)         - libc, libcpp, libssl, etc.
```

See [ARCHITECTURE.md](./ARCHITECTURE.md) for detailed design.

## Quick Start

### Building

```bash
# Clone repository
git clone https://github.com/vss-os/vss-co.git
cd vss-co

# Configure
mkdir build && cd build
cmake -G Ninja -DARCH=x86_64 ..

# Compile
ninja

# Run in QEMU (x86_64)
qemu-system-x86_64 -kernel kernel.bin -m 256M -serial stdio
```

See [BUILD.md](./BUILD.md) for detailed build instructions.

### Run Tests

```bash
ninja test
```

## Components

### Kernel (`kernel/`)

**Status**: Phase 1 - Early development

Core kernel functionality:
- CPU management (GDT, IDT, paging, context switching)
- Process management (fork, exec, exit, scheduler)
- Memory management (page allocator, virtual memory, swapping)
- Virtual filesystem (ext4, btrfs, tmpfs, procfs)
- Device drivers (console, block I/O, networking)
- IPC primitives (pipes, semaphores, signals)
- Synchronization (spinlocks, mutexes, RW locks)

**Supported Architectures**:
- ✓ x86-64 (primary)
- ▢ ARM64 (in progress)
- ▢ RISC-V (planned)
- ▢ PowerPC (planned)

### Shell (`shell/`)

**Status**: Phase 1 - Skeleton

GNU Bash-compatible shell with:
- Command parsing and execution
- Job control (foreground/background)
- Pipes and redirections
- Environment variables
- Built-in commands (cd, echo, export, etc.)
- History and tab completion
- Script execution

### System Utilities (`utils/`)

**Status**: Phase 2

Essential utilities:
- File operations (ls, cp, mv, rm, mkdir, etc.)
- Text processing (cat, grep, sed, awk, sort, etc.)
- System administration (ps, top, kill, chmod, chown, etc.)
- Networking (ping, ifconfig, ssh, telnet, etc.)
- Compression (gzip, tar, zip, etc.)
- Editors (nano, vi implementation)

### GUI (`gui/`)

**Status**: Phase 3 - Planned

Desktop environment:
- Window manager
- Graphics drivers and framebuffer
- Widget toolkit
- Desktop applications (terminal, file manager, text editor, system monitor)

### Package Manager (`pkgmgr/`)

**Status**: Phase 4 - Planned

Binary and source package management:
- Package format specification
- Repository management
- Dependency resolution
- Build system integration (cross-compilation)
- GPG signature verification
- Mirror selection and fallover

### Libraries (`lib/`)

**Status**: Ongoing

Core libraries:
- **libc**: POSIX-compatible C library (musl-based)
- **libcpp**: C++ standard library
- **libssl**: Cryptography (OpenSSL fork)
- **libcurl**: HTTP client library
- Media libraries (libpng, libz, etc.)

## Development Roadmap (10 Years)

### Year 1-2: Core Kernel + Shell ✓ (Starting)
- [x] Project structure & architecture
- [ ] Bootloader (GRUB2, UEFI, Coreboot)
- [ ] x86-64 kernel skeleton
- [ ] Process management
- [ ] Basic shell
- [ ] VFS + ext4
- [ ] QEMU testing

### Year 3-4: System Utilities + Drivers
- [ ] Network stack (IP, TCP, UDP)
- [ ] Block device drivers
- [ ] POSIX utilities (coreutils, util-linux)
- [ ] Compiler toolchain (GCC port)
- [ ] Build system (Make/CMake)

### Year 5-6: GUI + Desktop
- [ ] Framebuffer drivers
- [ ] Window manager
- [ ] Widget toolkit
- [ ] GTK/Qt ports
- [ ] Desktop applications

### Year 7-8: Package Manager + Distribution
- [ ] Package format
- [ ] Repository infrastructure
- [ ] Build system
- [ ] Release images
- [ ] Installation tools

### Year 9-10: Polish & Mobile
- [ ] ARM64/RISC-V optimization
- [ ] Android compatibility layer
- [ ] Performance hardening
- [ ] Security audits
- [ ] Documentation

## Coding Standards

All code must follow [CODING_STANDARDS.md](./CODING_STANDARDS.md):

- **C**: POSIX C17, K&R style with modern practices
- **C++**: C++20, RAII, STL containers
- **Go**: Standard Go conventions, gofmt compliance
- **Assembly**: Platform-specific, well-documented

## Contributing

1. Read [ARCHITECTURE.md](./ARCHITECTURE.md) and [CODING_STANDARDS.md](./CODING_STANDARDS.md)
2. Fork the repository
3. Create a feature branch (`git checkout -b feature/my-feature`)
4. Commit changes with clear messages (see standards)
5. Push to fork and submit pull request
6. Ensure all tests pass and code follows standards

## Testing

```bash
# Unit tests
ninja test

# Run valgrind (memory checking)
valgrind --leak-check=full ./build/shell

# QEMU functional tests
./scripts/test-qemu.sh
```

## Documentation

- [ARCHITECTURE.md](./ARCHITECTURE.md) - System architecture and design
- [CODING_STANDARDS.md](./CODING_STANDARDS.md) - Code guidelines
- [BUILD.md](./BUILD.md) - Build instructions
- [kernel/README.md](./kernel/README.md) - Kernel documentation
- [POSIX Compliance](./docs/POSIX.md) - POSIX standard implementation status

## License

MIT License - See [LICENSE](./LICENSE) for details.

This means you can:
- ✓ Use commercially
- ✓ Modify code
- ✓ Distribute
- ✓ Use privately

With the requirement of:
- ✓ License and copyright notice

## Community

- **GitHub Issues**: [Report bugs](https://github.com/vss-os/vss-co/issues)
- **Discussions**: [Ask questions](https://github.com/vss-os/vss-co/discussions)
- **IRC**: #vss-co on Libera.Chat
- **Mailing List**: discuss@vss-co.org

## System Requirements

### Minimum (Running)
- **CPU**: Any x86-64, ARM64, or RISC-V processor
- **RAM**: 256 MB
- **Disk**: 1 GB
- **Boot**: BIOS, UEFI, or custom bootloader

### Development (Building)
- **OS**: Linux, macOS, or Windows (MSYS2)
- **Compiler**: GCC 10+ or Clang 12+
- **Tools**: CMake 3.20+, Ninja 1.11+
- **Emulator**: QEMU 5.0+

## Performance Goals

Target performance metrics (Linux baseline = 100%):

| Benchmark | Target |
|-----------|--------|
| Process creation | 85% |
| Context switch | 90% |
| Memory allocation | 80% |
| File I/O | 88% |
| Scheduler overhead | 70% |

## Security

- **SELinux-like MAC**: Mandatory Access Control
- **Capabilities**: Fine-grained privilege system
- **ASLR**: Address Space Layout Randomization
- **Stack canaries**: Buffer overflow protection
- **W^X**: Write-exclusive OR execute-exclusive memory

## FAQ

**Q: Is this a Linux fork?**
A: No. We're building from scratch, learning from Linux design but with our own kernel architecture.

**Q: Will it run Linux binaries?**
A: Not directly, but we aim for >95% POSIX compatibility so porting is minimal.

**Q: How long will this take?**
A: 10 years is our estimate for a mature, production-ready system. Contributors speed this up.

**Q: Can I contribute?**
A: Yes! We welcome developers, documentation writers, and testers. See Contributing above.

**Q: What's the long-term goal?**
A: A viable, open-source alternative to Linux that's free, secure, and community-driven.

## Resources

- [OSDev Wiki](https://wiki.osdev.org/) - OS development reference
- [Linux Kernel Book](https://www.kernel.org/doc/html/latest/) - Kernel architecture
- [POSIX Standard](https://pubs.opengroup.org/onlinepubs/9699919799/) - POSIX specification
- [ARM64 ABI](https://github.com/ARM-software/abi-aa/) - ARM specification
- [Intel x86-64 Manual](https://www.intel.com/content/www/us/en/develop/articles/intel-sdm.html) - CPU reference

## Credits

- **Architecture inspired by**: Linux, UNIX, Darwin, Windows NT
- **Design principles**: POSIX, capability-based security, microkernel ideas
- **Community**: All contributors and maintainers

---

**Made with ❤️ for open-source advocates, OS enthusiasts, and the future of computing.**
