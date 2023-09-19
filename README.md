![Build status and tests](https://github.com/spiritEcosse/bible/actions/workflows/code_coverage.yml/badge.svg)
[![codecov](https://codecov.io/gh/spiritEcosse/bible/branch/master/graph/badge.svg)](https://codecov.io/gh/spiritEcosse/bible)
[![CodeFactor](https://www.codefactor.io/repository/github/spiritecosse/bible/badge)](https://www.codefactor.io/repository/github/spiritecosse/bible)
[![cpp-linter](https://github.com/spiritEcosse/bible/actions/workflows/cpp-linter.yml/badge.svg)](https://github.com/spiritEcosse/bible/actions/workflows/cpp-linter.yml)

# FreeBible

### Application bible for learning and daily reading for everyone.

# The application available on Operating Systems:

## [Sailfish OS](https://sailfishos.org/)

# The application available on architectures:

- armv7hl
- aarch64


## Other Operating Systems

Stay tuned for information on upcoming operating system releases. We will keep this section updated with details about new and exciting options for your device.

# Application Overview

The application is designed with three main pages:

## 1. Central Page - Modules by Translations

- This central page primarily focuses on displaying translations. Users can view one translation at a time, and there's an option to activate other translations for comparison or reference.
- At the bottom of this page, users can access their reading history, which includes a record of verses and books they have previously read.
- The navigation system within this page is user-friendly, allowing for easy movement between books and chapters within the translations.
- Offline reading functionality is supported, allowing users to access translations without an internet connection.
- Users can also access commentaries associated with the translations.

## 2. Left Page - Search by Verses

- The left page is dedicated to search functionality. Users can perform searches based on specific verses from the translations.
- When a user conducts a search, they can easily navigate to the translation page where the found verse is located for further exploration and context.

## 3. Right Page - List of Modules

- On the right page, users can manage the modules and translations available in the application.
- The system can download translations in various languages from the internet, expanding the available options.
- Users have the ability to remove translations they no longer need or want.
- Navigation within this page is designed to make it easy for users to switch between different modules.
- Additionally, users can search for specific modules by name, simplifying the process of locating and activating desired translations.

This three-page design offers a comprehensive and user-friendly experience for accessing, searching, and managing translations and related content within the application.

## 3. Screenshots
<img src="screenshots/sailfishos/sailfishos-harbour-freebible-1.jpeg" alt="sailfishos-harbour-freebible-1.jpeg" width="400" >
<img src="screenshots/sailfishos/sailfishos-harbour-freebible-2.jpeg" alt="sailfishos-harbour-freebible-2.jpeg" width="400" >
<img src="screenshots/sailfishos/sailfishos-harbour-freebible-3.jpeg" alt="sailfishos-harbour-freebible-3.jpeg" width="400" >

# Sailfish OS RPM Package Installation

This guide provides instructions on how to install RPM packages on Sailfish OS using various methods: via the Jolla Store, manually using `pkcon`, and manually using `zypper`.

## Installing RPM Packages via Jolla Store

1. Open the Jolla Store on your Sailfish device.

2. Search for the desired application or package.

3. On the package details page, click the "Install" button.

4. Follow the on-screen prompts to complete the installation.

## Installing RPM Packages Manually using `pkcon`

1. Transfer the RPM package file to your Sailfish device using methods like email, cloud storage, or USB connection.

2. Open the Terminal app on your Sailfish device.

3. Navigate to the directory containing the RPM package:

   ```bash
   cd /path/to/your/package/
   ```

4. Install the RPM package using `pkcon`:

   ```bash
   pkcon install-local filename.rpm
   ```

   Replace `filename.rpm` with the actual name of your RPM package file.

5. Enter your password when prompted to authorize the installation.

6. Wait for the installation to complete, and you should see a confirmation message.

## Installing RPM Packages Manually using `zypper`

**Note**: If `zypper` is not already installed on your device, follow these steps to install it:

1. Open the Terminal app on your Sailfish device.

2. To enable developer mode, set a password:

   ```bash
   devel-su
   ```

   Follow the prompts to set a password for developer mode.

3. Update the package manager and install `zypper`:

   ```bash
   pkcon refresh
   pkcon install zypper
   ```

   Follow any prompts and provide your password when requested.

4. Once `zypper` is installed, you can use it to install RPM packages:

   ```bash
   zypper install --details /path/to/your/package/filename.rpm
   ```

   Replace `/path/to/your/package/filename.rpm` with the actual path to your RPM package.

Please ensure that you handle dependencies appropriately when using manual installation methods (`pkcon` and `zypper`), as these methods may require more manual management of dependencies compared to the Jolla Store.


## Compatibility

This project has been tested on the following versions of Sailfish OS:

- 4.5.0.19

# Running tests

## Unix OS

```bash
# e - script stops on error (return != 0)
# u - error if undefined variable
# o pipefail - script fails if one of piped command fails
# x - output each line (debug)

set -euox pipefail
git clone https://github.com/spiritEcosse/bible
export FREEBIBLE_PATH_BUILD=`pwd`/bible_build
export FREEBIBLE_PATH_SRC=`pwd`/bible
mkdir ${FREEBIBLE_PATH_BUILD}
cd ${FREEBIBLE_PATH_BUILD}
cmake -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTING=ON -S ${FREEBIBLE_PATH_SRC} -B ${FREEBIBLE_PATH_BUILD}
cmake --build ${FREEBIBLE_PATH_BUILD} -j "$((2 * $(getconf _NPROCESSORS_ONLN)))"
ctest --output-on-failure
```

# TODO

- Adding settings
- Support commentaries
- Support cover page for Sailfish OS
  - display a spinner during the post-installation process following the application installation
  - show status of loading list of modules
  - show status of loading modules
  - show count of loading modules
  - show verses
  - add carousel for 3 pages
- Adding bookmarks
- Adding dictionaries
- Adding notes
- Adding audio notes
- Convert audio to text
- Auditory presentation of textual information
- Adding fonts
- Support strong numbers
- Adding notifications


## Support all modern platforms

- Linux 
- Mac OS 
- Windows
- Android
- iOS

## Responsive Design

- Tablet
- Desktop
- Mobile


## Embrace all popular configurations architectures


# Participation

If you're interested in getting involved and contributing to the development and maintenance of this application, please don't hesitate to reach out. Your help would be greatly appreciated!

## Contact

You can contact me via email at [shevchenkcoigor@gmail.com](mailto:shevchenkcoigor@gmail.com) for further information or inquiries.

# Licensing

This project is released under the GNU General Public License v3.0 (GPL-3.0), a widely used open-source software license.

<a href="https://www.buymeacoffee.com/shevchenkco" target="_blank"><img src="https://cdn.buymeacoffee.com/buttons/default-orange.png" alt="Buy Me A Coffee" height="41" width="174"></a>
