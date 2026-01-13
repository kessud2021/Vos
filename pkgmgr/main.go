package main

import (
	"flag"
	"fmt"
	"os"
	"vss-co/pkgmgr/cmd"
	"vss-co/pkgmgr/config"
)

func main() {
	if len(os.Args) < 2 {
		printUsage()
		os.Exit(1)
	}

	subcommand := os.Args[1]

	switch subcommand {
	case "install", "i":
		cmd.CmdInstall(os.Args[2:])
	case "remove", "rm":
		cmd.CmdRemove(os.Args[2:])
	case "update":
		cmd.CmdUpdate(os.Args[2:])
	case "search":
		cmd.CmdSearch(os.Args[2:])
	case "list":
		cmd.CmdList(os.Args[2:])
	case "info":
		cmd.CmdInfo(os.Args[2:])
	case "repo":
		cmd.CmdRepo(os.Args[2:])
	case "clean":
		cmd.CmdClean(os.Args[2:])
	case "sync":
		cmd.CmdSync(os.Args[2:])
	case "version", "-v":
		fmt.Println("VSS-CO Package Manager v1.0")
	case "help", "-h":
		printUsage()
	default:
		fmt.Fprintf(os.Stderr, "Unknown command: %s\n", subcommand)
		os.Exit(1)
	}
}

func printUsage() {
	usage := `VSS-CO Package Manager v1.0

Usage:
  pkgmgr [command] [options]

Commands:
  install <pkg>      Install package(s)
  remove  <pkg>      Remove package(s)
  update  [pkg]      Update package(s)
  search  <query>    Search packages
  list               List installed packages
  info    <pkg>      Show package information
  repo    <action>   Manage repositories
  sync               Sync package database
  clean              Clean cache
  version            Show version
  help               Show this help

Examples:
  pkgmgr install vim emacs          # Install multiple packages
  pkgmgr search web                 # Search for "web" packages
  pkgmgr update                     # Update all packages
  pkgmgr remove curl                # Remove curl

Use 'pkgmgr help <command>' for more info.
`
	fmt.Print(usage)
}

// Package configuration
var cfg *config.Config

func init() {
	var err error
	cfg, err = config.LoadConfig()
	if err != nil {
		fmt.Fprintf(os.Stderr, "Failed to load config: %v\n", err)
		os.Exit(1)
	}
}
