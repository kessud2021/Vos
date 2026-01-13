.PHONY: help build run clean rebuild test all

# Default target
.DEFAULT_GOAL := help

# Colors
RED := \033[0;31m
GREEN := \033[0;32m
YELLOW := \033[1;33m
NC := \033[0m

# Directories
BUILD_DIR := build
KERNEL_BINARY := $(BUILD_DIR)/test-kernel.exe
OS_IMAGE := $(BUILD_DIR)/os

help: ## Show this help message
	@echo "╔════════════════════════════════════════════════════╗"
	@echo "║         VSS-CO OS - Production Build System         ║"
	@echo "╚════════════════════════════════════════════════════╝"
	@echo ""
	@echo "Usage: make [target]"
	@echo ""
	@echo "Targets:"
	@awk 'BEGIN {FS = ":.*?## "} /^[a-zA-Z_-]+:.*?## / {printf "  $(GREEN)%-20s$(NC) %s\n", $$1, $$2}' $(MAKEFILE_LIST)
	@echo ""
	@echo "Examples:"
	@echo "  make build          # Build the OS"
	@echo "  make run            # Run the kernel"
	@echo "  make clean          # Clean build artifacts"
	@echo "  make rebuild        # Clean and rebuild"
	@echo "  make all            # Build and run"
	@echo ""

build: ## Build the OS (cmake + ninja)
	@echo "$(YELLOW)Building VSS-CO OS...$(NC)"
	@if [ ! -d "$(BUILD_DIR)" ]; then mkdir -p $(BUILD_DIR); fi
	@cd $(BUILD_DIR) && cmake -G Ninja -DARCH=x86_64 .. && ninja
	@echo "$(GREEN)✓ Build complete$(NC)"
	@echo "  Kernel: $(KERNEL_BINARY)"
	@echo "  Image:  $(OS_IMAGE)"

run: build ## Run the kernel test harness
	@echo ""
	@echo "$(YELLOW)Running VSS-CO OS Kernel...$(NC)"
	@echo "$(YELLOW)════════════════════════════════════════════$(NC)"
	@./$(KERNEL_BINARY)

test: build ## Run tests
	@echo "$(YELLOW)Running tests...$(NC)"
	@cd $(BUILD_DIR) && ninja test

clean: ## Clean build artifacts
	@echo "$(YELLOW)Cleaning build artifacts...$(NC)"
	@rm -rf $(BUILD_DIR)
	@echo "$(GREEN)✓ Clean complete$(NC)"

rebuild: clean build ## Clean and rebuild
	@echo "$(GREEN)✓ Rebuild complete$(NC)"

all: build run ## Build and run the OS

info: ## Show build info
	@echo "VSS-CO OS Build Information"
	@echo "  Project: VSS-CO OS"
	@echo "  Version: 1.0"
	@echo "  Status:  Production"
	@echo ""
	@echo "Source Files:"
	@find . -name "*.c" -o -name "*.h" | wc -l | xargs echo "  C/Header files:"
	@find . -name "*.s" | wc -l | xargs echo "  Assembly files:"
	@echo ""
	@echo "Build Configuration:"
	@echo "  Build System: CMake + Ninja"
	@echo "  C Standard: C17"
	@echo "  C++ Standard: C++20"
	@echo "  Architectures: x86-64, ARM64, RISC-V (ready)"
	@echo ""

format: ## Format code with clang-format
	@echo "$(YELLOW)Formatting code...$(NC)"
	@find kernel shell utils lib -name "*.c" -o -name "*.h" | xargs clang-format -i
	@echo "$(GREEN)✓ Formatting complete$(NC)"

docs: ## Generate documentation
	@echo "$(YELLOW)Generating documentation...$(NC)"
	@echo "See ARCHITECTURE.md, ROADMAP.md, PRODUCTION_STATUS.txt"

status: ## Show project status
	@echo "$(GREEN)VSS-CO OS Status$(NC)"
	@echo ""
	@echo "✅ Implemented:"
	@echo "  • Buddy allocator (memory management)"
	@echo "  • Slab allocator (kmalloc/kfree)"
	@echo "  • Process scheduler (fork, scheduling)"
	@echo "  • Virtual filesystem (VFS)"
	@echo "  • Shell (bash-compatible)"
	@echo "  • Boot sequence"
	@echo ""
	@echo "⬜ Coming Soon:"
	@echo "  • Network stack"
	@echo "  • Graphics/GUI"
	@echo "  • Package manager"
	@echo "  • Security (MAC/ACL)"
	@echo "  • Multi-core support"
	@echo ""

install: build ## Install to system (NOT YET)
	@echo "Installation not yet supported"
	@echo "See ROADMAP.md for Phase 4"

.PHONY: help build run clean rebuild test all info format docs status install
