# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](http://keepachangelog.com/en/1.0.0/)
and this project adheres to [Semantic Versioning](http://semver.org/spec/v2.0.0.html).

## Unversionned

### Added

* Template to build new backends.

## [2.0.0] - 2020-05-13

### Added

* A small features function and enum to check what the backend supports
* Header only target in build system
* Clang integration
* New CMake files to find Tango

### Changed

* Renamed API to suit actual functionality
* Code refactor:
  * Header install path is hdb++ (this is a generic path that other projects will use)
  * Split single source/header into client/backend headers.
  * Updated build system (multiple project integration work)
  * Modernize c++ code.
  * Observe namespaces in headers
  * Added code to hdbpp namespace
  * Pass by reference for performance where possible without changing API
* Combined README and INSTALL files
* Removed TTL from add attribute function.

### Removed

* Some unused functions.
* HdbClientData class, this should be in the Event Subscriber

## [1.0.1] - 2019-09-23

### Changed

* Minor document updates

## [1.0.0] - 2017-09-13

### Added

* CHANGELOG.md file.
* INSTALL.md file.
* LICENCE file.
* License headers in code.
* Debian Package build files under debian/
* CMake build and configuration files.

### Changed

* Moved build system from Make to CMake.
* README.md - Added lots of new information.
* Source file headers changed for correct licence.

### Removed

* Makefile build system.
