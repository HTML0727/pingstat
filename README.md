# Ping-Status

**A modern command-line HTTP request analyzer tool that displays detailed timing information and status codes with a beautiful interface.**

**For Chinese documentation, please see [ZH.README.md.](ZH.README.md)**

## Features

- Detailed HTTP timing breakdown (DNS, TCP, TLS, Server Processing, Content Transfer)
- Beautiful, themeable interface inspired by btop
- Multiple color themes (Tokyo Night, Adiwata, Apple System)
- Real-time progress visualization
- History chart for tracking performance over time
- JSON output support for scripting
- Continuous monitoring mode

## Installation

### Prerequisites

- **libcurl development libraries**
- **ncurses development libraries**
- **json-c development libraries**

    # On Ubuntu/Debian:
    ```bash
    sudo apt-get install libcurl4-openssl-dev libncursesw5-dev libjson-c-dev
    ```

    # On CentOS/RHEL:
    ```bash
    sudo yum install libcurl-devel ncurses-devel json-c-devel
    ```

   # On Arch Linux:
   ```bash
   sudo pacman -S base-devel ncurses cjson
    ```

   # On Gentoo Linux:
   ```bash
   sudo emerge dev-libs/ncurses dev-libs/cjson
    ```

   # On macOS with Homebrew:
   ```terminal
   brew install ncurses cjson
   ```

### Compile from source code
```bash
git clone https://github.com/HTML0727/ping-status.git
cd ping-status
make
sudo make install
```

## Configuration
Ping-Status uses configuration files located in ```~/.config/ping-status/.``` On first run, it will create this directory with default configuration files if they don't exist.

### Configuration Files
```~/.config/ping-status/config.conf```

Main configuration file for application behavior:

```ini
# Ping-Status Configuration File
# This file controls the behavior of the ping-status application

[general]
# Default URL to check when none is provided
# default_url = https://example.com
# Default check interval in seconds (for continuous mode)
interval = 5
# Default number of requests to make
count = 1
# Default output format (text, json)
output_format = text
# Default theme (tokyo-night, adiwata, apple-system, custom)
theme = tokyo-night
# Whether to verify SSL certificates (0 = no, 1 = yes)
verify_ssl = 1
# Timeout for HTTP requests in seconds
timeout = 10
# User agent string to use for requests
user_agent = ping-status/1.0

[display]
# Refresh rate for the UI in milliseconds
refresh_rate = 100
# Whether to show progress bars
show_progress_bars = 1
# Whether to show history chart
show_history = 1
# Maximum number of history entries to keep
max_history_entries = 50
# Whether to show detailed timing information
show_detailed_timing = 1

[behavior]
# Whether to follow redirects
follow_redirects = 1
# Maximum number of redirects to follow
max_redirects = 10
# Whether to include response body in timing (0 = headers only, 1 = full response)
include_body = 1
# Whether to resolve DNS for display
resolve_dns = 1
```

```~/.config/ping-status/css.conf```

CSS-based theme configuration:

```css
/* Ping-Status Theme Configuration */
/* This file controls the visual appearance of ping-status */

:root {
  /* Base colors - used by all themes unless overridden */
  --primary-color: #1a1b26;
  --secondary-color: #24283b;
  --accent-color: #bb9af7;
  --background-color: #16161e;
  --text-color: #a9b1d6;
  --success-color: #73daca;
  --warning-color: #e0af68;
  --error-color: #f7768e;
  --border-color: #414868;
  --highlight-color: #7aa2f7;
}

/* Tokyo Night Theme (default) */
.tokyo-night {
  --primary-color: #1a1b26;
  --secondary-color: #24283b;
  --accent-color: #bb9af7;
  --background-color: #16161e;
  --text-color: #a9b1d6;
  --success-color: #73daca;
  --warning-color: #e0af68;
  --error-color: #f7768e;
  --border-color: #414868;
  --highlight-color: #7aa2f7;
}

/* Adiwata Theme */
.adiwata {
  --primary-color: #2a2b36;
  --secondary-color: #343746;
  --accent-color: #6d8cbd;
  --background-color: #22232e;
  --text-color: #c5c8d1;
  --success-color: #8dc776;
  --warning-color: #e6b455;
  --error-color: #e35555;
  --border-color: #44475a;
  --highlight-color: #5e81ac;
}

/* Apple System Theme */
.apple-system {
  --primary-color: #ffffff;
  --secondary-color: #f5f5f7;
  --accent-color: #007aff;
  --background-color: #ffffff;
  --text-color: #333333;
  --success-color: #34c759;
  --warning-color: #ff9500;
  --error-color: #ff3b30;
  --border-color: #c6c6c8;
  --highlight-color: #007aff;
}

/* Custom Theme - uncomment and modify to create your own theme */
/*
.custom-theme {
  --primary-color: #your-color;
  --secondary-color: #your-color;
  --accent-color: #your-color;
  --background-color: #your-color;
  --text-color: #your-color;
  --success-color: #your-color;
  --warning-color: #your-color;
  --error-color: #your-color;
  --border-color: #your-color;
  --highlight-color: #your-color;
}
*/

/* UI Elements */
.status-container {
  background-color: var(--background-color);
  color: var(--text-color);
  border: 1px solid var(--border-color);
  border-radius: 8px;
  padding: 1rem;
  margin: 0.5rem;
}

.status-header {
  color: var(--accent-color);
  border-bottom: 1px solid var(--border-color);
  padding-bottom: 0.5rem;
  margin-bottom: 1rem;
  font-weight: bold;
}

.timing-item {
  display: flex;
  justify-content: space-between;
  padding: 0.25rem 0;
  border-bottom: 1px solid var(--secondary-color);
}

.timing-value {
  font-weight: bold;
}

.timing-value.good {
  color: var(--success-color);
}

.timing-value.warning {
  color: var(--warning-color);
}

.timing-value.bad {
  color: var(--error-color);
}

.progress-bar {
  height: 8px;
  background-color: var(--secondary-color);
  border-radius: 4px;
  overflow: hidden;
  margin-top: 0.25rem;
}

.progress-fill {
  height: 100%;
  background-color: var(--accent-color);
  transition: width 0.3s ease;
}

.progress-fill.good {
  background-color: var(--success-color);
}

.progress-fill.warning {
  background-color: var(--warning-color);
}

.progress-fill.bad {
  background-color: var(--error-color);
}

.http-status {
  display: inline-block;
  padding: 0.25rem 0.5rem;
  border-radius: 4px;
  font-weight: bold;
}

.http-status-2xx {
  background-color: var(--success-color);
  color: var(--background-color);
}

.http-status-3xx {
  background-color: var(--warning-color);
  color: var(--background-color);
}

.http-status-4xx, .http-status-5xx {
  background-color: var(--error-color);
  color: var(--background-color);
}

.history-chart {
  height: 60px;
  background-color: var(--secondary-color);
  border-radius: 4px;
  display: flex;
  align-items: flex-end;
  padding: 0.25rem;
  gap: 2px;
}

.history-bar {
  background-color: var(--accent-color);
  border-radius: 2px 2px 0 0;
  flex: 1;
  min-width: 2px;
}

.history-bar.good {
  background-color: var(--success-color);
}

.history-bar.warning {
  background-color: var(--warning-color);
}

.history-bar.bad {
  background-color: var(--error-color);
}

.control-button {
  background-color: var(--secondary-color);
  border: 1px solid var(--border-color);
  color: var(--text-color);
  padding: 0.5rem 1rem;
  border-radius: 4px;
  cursor: pointer;
  margin-right: 0.5rem;
}

.control-button:hover {
  background-color: var(--accent-color);
  color: var(--background-color);
}

.theme-button {
  width: 20px;
  height: 20px;
  border-radius: 50%;
  border: 2px solid var(--border-color);
  cursor: pointer;
  display: inline-block;
  margin-right: 0.5rem;
}

.theme-button.active {
  border-color: var(--accent-color);
}

.theme-button.tokyo-night {
  background-color: #bb9af7;
}

.theme-button.adiwata {
  background-color: #6d8cbd;
}

.theme-button.apple-system {
  background-color: #007aff;
}

.loading {
  opacity: 0.7;
  pointer-events: none;
}

.loading::after {
  content: '';
  display: inline-block;
  width: 12px;
  height: 12px;
  border: 2px solid transparent;
  border-top: 2px solid var(--accent-color);
  border-radius: 50%;
  animation: spin 1s linear infinite;
  margin-left: 8px;
}

@keyframes spin {
  0% { transform: rotate(0deg); }
  100% { transform: rotate(360deg); }
}
```

## Usage

### Basic  Usage:
```bash
ping-status https://example.com
```

### With Options:
```bash
# Custom interval between requests
ping-status -i 10 https://example.com

# Continuous monitoring
ping-status -C https://example.com

# JSON output format
ping-status -o json https://example.com

# Custom theme
ping-status -t adiwata https://example.com

# Multiple requests
ping-status -c 5 https://example.com
```

### Command Line Options:
- ```-i, --interval SECONDS``` : Interval between requests (default: 5)
- ```-c, --count NUMBER``` : Number of requests to make (default: 1)
- ```-C, --continuous``` : Continuous mode (until interrupted)
- ```-v, --verbose``` :  Verbose output
- ```-t, --theme THEME``` : Color theme (tokyo-night, adiwata, apple-system, custom)
- ```-o, --output FORMAT``` : Output format (text, json)
- ```-h, --help``` : Show help message

### Interactive Controls

When running in interactive mode (the default), you can use these keyboard shortcuts:
- ```r``` : Refresh immediately
- ```t``` : Cycle through themes
- ```h``` : Show help
- ```q``` : Quit (in continuous mode)

## Themes
Ping-Status comes with three built-in themes:
1. **Tokyo Night**: A dark theme inspired by Tokyo City lights (default)
2. **Adiwata**: A balanced dark theme with blue accents
3. **Apple System**: A light theme mimicking macOS system colors

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request. For major changes, please open an issue first to discuss what you would like to change.
## License
This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments
- Inspired by httpstat and similar HTTP debugging tools
-  UI design inspired by btop and other modern terminal applications
- Color themes inspired by popular color schemes like Tokyo Night
