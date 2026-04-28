#!/bin/bash

# ==================== ERROR SYMBOLS REFERENCE ====================

# ❌ - Critical error / Failure
# ⚠️  - Warning / Non-critical issue
# ✅ - Success / Completed
# ℹ️  - Information
# 🔧 - Configuration / Setup
# 🚀 - Deployment / Starting process
# 📦 - Package / Installation
# 🔍 - Searching / Checking
# 💡 - Tip / Suggestion
# 🛑 - Stop / Halt execution
# ⏳ - Waiting / In progress
# ✨ - New feature / Enhancement
# 🐛 - Bug / Issue
# 🔒 - Security / Permission issue
# 🌐 - Network / Remote operation
# 📁 - File / Directory operation
# 🔄 - Retry / Loop / Update
# ⚡ - Performance / Quick action
# 📊 - Statistics / Results
# 🎯 - Target / Goal

# ==================== ASCII ERROR INDICATORS ====================

# [ERROR]    or  ERROR:    - Critical error / Failure
# [WARN]     or  WARNING:  - Warning / Non-critical issue
# [OK]       or  SUCCESS:  - Success / Completed
# [INFO]     or  INFO:     - Information
# [CONFIG]   or  CFG:      - Configuration / Setup
# [DEPLOY]   or  DEPLOY:   - Deployment / Starting process
# [CHECK]    or  CHECK:    - Searching / Checking
# [TIP]      or  TIP:      - Suggestion
# [STOP]     or  STOP:     - Halt execution
# [WAIT]     or  WAIT:     - Waiting / In progress
# [SECURITY] or  SEC:      - Security issue

# ASCII Art Symbols (when you want visual indicators)
# [X]        - Error / Failure
# [√]        - Success (if terminal supports)
# [!]        - Warning / Important
# [i]        - Information
# [>]        - Progress / Next step
# [*]        - Generic bullet point

check_echo_color() {
    # 定义颜色常量
    # 8色（标准）
    BLACK='\033[0;30m'
    DARK_GRAY='\033[1;30m'
    RED='\033[0;31m'
    LIGHT_RED='\033[1;31m'
    GREEN='\033[0;32m'
    LIGHT_GREEN='\033[1;32m'
    BROWN='\033[0;33m'
    YELLOW='\033[1;33m'
    BLUE='\033[0;34m'
    LIGHT_BLUE='\033[1;34m'
    PURPLE='\033[0;35m'
    LIGHT_PURPLE='\033[1;35m'
    CYAN='\033[0;36m'
    LIGHT_CYAN='\033[1;36m'
    LIGHT_GRAY='\033[0;37m'
    WHITE='\033[1;37m'
    # 256色（需要终端支持）
    # 格式：\033[38;5;${color}m 前景色
    # 格式：\033[48;5;${color}m 背景色
    # 背景颜色（在颜色代码基础上加 10）
    BG_BLACK='\033[40m'
    BG_RED='\033[41m'
    BG_GREEN='\033[42m'
    BG_YELLOW='\033[43m'
    BG_BLUE='\033[44m'
    BG_PURPLE='\033[45m'
    BG_CYAN='\033[46m'
    BG_WHITE='\033[47m'
    # 样式
    BOLD='\033[1m'
    UNDERLINE='\033[4m'
    BLINK='\033[5m'  # 闪烁（部分终端不支持）
    REVERSE='\033[7m'  # 反显
    # 重置
    NC='\033[0m'  # No Color

    # 检查是否支持颜色
    if [ -t 1 ] && [ "$TERM" != "dumb" ]; then
        # 支持颜色
        RED='\033[0;31m'
        GREEN='\033[0;32m'
        NC='\033[0m'
    else
        # 不支持颜色
        RED=''
        GREEN=''
        NC=''
    fi

    echo -e "${GREEN}带颜色的输出${NC}"
}

check_echo_color_with_tput() {
    # 使用 tput（更兼容）
    if command -v tput >/dev/null 2>&1 && [ -t 1 ]; then
        BOLD=$(tput bold)
        RED=$(tput setaf 1)
        GREEN=$(tput setaf 2)
        YELLOW=$(tput setaf 3)
        BLUE=$(tput setaf 4)
        MAGENTA=$(tput setaf 5)
        CYAN=$(tput setaf 6)
        NC=$(tput sgr0)
    else
        BOLD=""; RED=""; GREEN=""; YELLOW=""; BLUE=""; MAGENTA=""; CYAN=""; NC=""
    fi
}
log_info() {
    echo -e "[i] $(date '+%Y-%m-%d %H:%M:%S') - $1"
}
log_warn() {
    echo -e "${YELLOW}[!]${NC} $(date '+%Y-%m-%d %H:%M:%S') - $1"
}
log_error() {
    echo -e "${RED}[X]${NC} $(date '+%Y-%m-%d %H:%M:%S') - $1" >&2
}
log_success() {
    echo -e "${GREEN}[✓]${NC} $(date '+%Y-%m-%d %H:%M:%S') -  $1"
}
echo_success() {
    echo -e "${GREEN}[✓]${NC} $1"
}
echo_error() {
    echo -e "${RED}[X]${NC} $1" >&2
}
echo_warn() {
    echo -e "${YELLOW}[!]${NC} $1"
}
echo_info() {
    echo -e "[i] $1"
}
echo_status() {
    echo "=== $1"
}
echo_color_success() {
    echo -e "${GREEN}$1${NC}"
}
echo_color_warn() {
    echo -e "${YELLOW}$1${NC}"
}
echo_color_error() {
    echo -e "${RED}$1${NC}" >&2
}

# Validate directory path
validate_directory() {
    local dir="$1"
    local type="$2"

    if [ -n "$dir" ]; then
        if [[ "$dir" =~ ^- ]]; then
            echo_error "Error: $type directory parameter '$dir' cannot start with '-'"
            exit 1
        fi
    fi
}

# 详细的 Linux 发行版检测函数
get_linux_distro() {
    if [ -f /etc/os-release ]; then
        . /etc/os-release
        echo "$ID"
    elif [ -f /etc/redhat-release ]; then
        if grep -q "CentOS" /etc/redhat-release; then
            echo "centos"
        elif grep -q "Red Hat" /etc/redhat-release; then
            echo "rhel"
        elif grep -q "Fedora" /etc/redhat-release; then
            echo "fedora"
        else
            echo "redhat"
        fi
    elif [ -f /etc/debian_version ]; then
        echo "debian"
    elif [ -f /etc/arch-release ]; then
        echo "arch"
    elif [ -f /etc/SuSE-release ]; then
        echo "suse"
    elif [ -f /etc/alpine-release ]; then
        echo "alpine"
    else
        echo "unknown"
    fi
}

# 获取版本号
get_linux_version() {
    if [ -f /etc/os-release ]; then
        . /etc/os-release
        echo "$VERSION_ID"
    elif [ -f /etc/redhat-release ]; then
        grep -oE '[0-9.]+' /etc/redhat-release | head -1
    elif [ -f /etc/debian_version ]; then
        cat /etc/debian_version
    else
        echo "unknown"
    fi
}

# 检测包管理器
get_package_tool() {
    local distro=$1
    case $distro in
        ubuntu|debian|kali)
            echo "apt"
            ;;
        deepin)
            echo "apt-get"
            ;;
        fedora|rhel|centos|almalinux|rocky)
            if command -v dnf >/dev/null 2>&1; then
                echo "dnf"
            else
                echo "yum"
            fi
            ;;
        arch|manjaro)
            echo "pacman"
            ;;
        opensuse*|sles)
            echo "zypper"
            ;;
        alpine)
            echo "apk"
            ;;
        *)
            # 尝试自动检测可用的包管理器
            if command -v apt >/dev/null 2>&1; then
                echo "apt"
            elif command -v dnf >/dev/null 2>&1; then
                echo "dnf"
            elif command -v yum >/dev/null 2>&1; then
                echo "yum"
            elif command -v pacman >/dev/null 2>&1; then
                echo "pacman"
            elif command -v zypper >/dev/null 2>&1; then
                echo "zypper"
            elif command -v apk >/dev/null 2>&1; then
                echo "apk"
            else
                echo "unknown"
            fi
            ;;
    esac
}

package_install() {
    local PACKAGE=$@
    if [ -n "$PACKAGE" ]; then
        #if [ "$BUILD_VERBOSE" = "ON" ]; then
        #    echo "Install package: $PACKAGE"
        #fi
        for p in $PACKAGE
        do
            case "$PACKAGE_TOOL" in
                brew)
                    brew install -q $p
                    ;;
                apt)
                    if [ "$BUILD_VERBOSE" = "ON" ]; then
                        ${PACKAGE_TOOL} install -y -q $p
                    else
                        ${PACKAGE_TOOL} install -y -qq $p
                    fi
                    ;;
                dnf|yum)
                    ${PACKAGE_TOOL} install -y $p
                    ;;
                *)
                    ${PACKAGE_TOOL} install -y $p
                    ;;
            esac
        done
    fi
}

create_debian_folder() {
    local repo_root=$1
    if [ ! -d "$repo_root/debian" ]; then
        echo_status "Create $repo_root/debian ......"
        ln -s $repo_root/Package/debian $repo_root/debian

        if [ ! -f "$repo_root/debian/control" ]; then
            case "${DISTRO}:${DISTRO_VERSION}" in
                ubuntu:26.*|ubuntu:25.*)
                    control_source="$repo_root/Package/debian/control.ubuntu.26"
                    ;;
                debian:12)
                    control_source="$repo_root/Package/debian/control.debian.12"
                    ;;
                debian:13)
                    control_source="$repo_root/Package/debian/control.debian.13"
                    ;;
                deepin*)
                    control_source="$repo_root/Package/debian/control.deepin.23"
                    ;;
                *)
                    control_source="$repo_root/Package/debian/control.default"
                    ;;
            esac
            if [ -f "$control_source" ]; then
                ln -s $control_source $repo_root/debian/control
                if [ "$BUILD_VERBOSE" = "ON" ]; then
                    echo "ln -s $control_source $repo_root/debian/control"
                fi
            else
                echo_error "Error: $control_source is not exist"
            fi
        fi

    fi
}

install_debian_depend() {
    local repo_root=$1
    create_debian_folder $repo_root
    if [ -f "$repo_root/debian/control" ]; then
        echo_status "Install deb depends ......"
        pushd $repo_root
        # 安装编译依赖到系统中
        apt-get build-dep -y .
        popd
        # 创建虚拟包来管理依赖， -r : 在编译完成后再删除
        #mk-build-deps -i -r --tool 'apt-get -y' "$repo_root/debian/control"
#        grep-dctrl -s Build-Depends -n . $repo_root/debian/control | \
#            sed 's/([^)]*)//g' | \
#            sed 's/,/\n/g' | \
#            awk '{print $1}' | \
#            grep -v '^$' | \
#            xargs sudo apt install -y
    else
        echo_error "Error: $repo_root/debian/control is not exist"
    fi
}

# 完整的系统信息函数
get_system_info() {
    local os
    local distro
    local version
    local pkg_tool

    case "$(uname -s)" in
        Darwin*)
            os="macOS"
            distro="macOS"
            version=$(sw_vers -productVersion)
            pkg_tool="brew"
            ;;
        Linux*)
            os="Linux"
            distro=$(get_linux_distro)
            version=$(get_linux_version)
            pkg_tool=$(get_package_tool "$distro")
            ;;
        CYGWIN*|MINGW*|MSYS*)
            os="Windows"
            distro="windows"
            version=$(uname -r)
            pkg_tool="choco"  # Chocolatey
            ;;
        *)
            os="Unknown"
            distro="unknown"
            version="unknown"
            pkg_tool="unknown"
            ;;
    esac

    echo "OS: $os"
    echo "Distribution: $distro"
    echo "Version: $version"
    echo "Package tool: $pkg_tool"
    echo "Architecture: $(uname -m)"
}

install_gnu_getopt() {
    if command -v getopt > /dev/null 2>&1; then
        return
    fi
    case "$DISTRO" in
        fedora)
            package_install util-linux
            ;;
        macOS)
            # 检查　GNU setopt
            # 在 macOS 上，本地 getopt 不支持长格式参数，所以需要先在系统上安装 GNU getopt，并设置环境变量 PATH
            #if [ ! -f /usr/local/opt/gnu-getopt/bin/getopt ]; then
            #    brew install gnu-getopt
            #fi
            #if [[ ! "$PATH" =~ /usr/local/opt/gnu-getopt/bin/getopt/bin ]]; then
            #    export PATH=/usr/local/opt/gnu-getopt/bin/getopt/bin::$PATH
            #fi
            ;;
            *)
            ;;
    esac
}

# macOS 设置函数
setup_macos() {
    echo_status "Setting up macOS environment..."

    # 检查是否安装了 Homebrew
    if ! command -v brew >/dev/null 2>&1; then
        echo_status "Installing Homebrew..."
        /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
    fi

    # 确保 Homebrew 在 PATH 中
    if [ -f "/opt/homebrew/bin/brew" ]; then
        eval "$(/opt/homebrew/bin/brew shellenv)"
    elif [ -f "/usr/local/bin/brew" ]; then
        eval "$(/usr/local/bin/brew shellenv)"
    fi

    install_gnu_getopt
}

# 安全的 readlink 函数，兼容各种系统
safe_readlink() {
    local path="$1"
    if [ -L "$path" ]; then
        if command -v readlink >/dev/null 2>&1; then
            if readlink -f "$path" >/dev/null 2>&1; then
                readlink -f "$path"
            else
                readlink "$path"
            fi
        else
            ls -l "$path" | awk '{print $NF}'
        fi
    elif [ -e "$path" ]; then
        if command -v realpath >/dev/null 2>&1; then
            realpath "$path"
        else
            echo "$(cd "$(dirname "$path")" && pwd)/$(basename "$path")"
        fi
    else
        echo "$path"
    fi
}

# 检测操作系统并设置环境变量
detect_os_info() {
    local system_info
    system_info=$(get_system_info)

    export OS=$(echo "$system_info" | grep "^OS:" | cut -d':' -f2 | xargs)
    export DISTRO=$(echo "$system_info" | grep "^Distribution:" | cut -d':' -f2 | xargs)
    export DISTRO_VERSION=$(echo "$system_info" | grep "^Version:" | cut -d':' -f2 | xargs)
    export PACKAGE_TOOL=$(echo "$system_info" | grep "^Package tool:" | cut -d':' -f2 | xargs)
    export ARCHITECTURE=$(echo "$system_info" | grep "^Architecture:" | cut -d':' -f2 | xargs)
#    # 使用 awk 安全地提取值（处理空格）
#    export OS=$(echo "$system_info" | grep "^OS:" | awk -F': ' '{print $2}')
#    export DISTRO=$(echo "$system_info" | grep "^Distribution:" | awk -F': ' '{print $2}')
#    export DISTRO_VERSION=$(echo "$system_info" | grep "^Version:" | awk -F': ' '{print $2}')
#    export PACKAGE_TOOL=$(echo "$system_info" | grep "^Package tool:" | awk -F': ' '{print $2}')
#    export ARCHITECTURE=$(echo "$system_info" | grep "^Architecture:" | awk -F': ' '{print $2}')

    # 如果是 macOS，进行初始化设置
    if [ "$DISTRO" = "macOS" ]; then
        setup_macos
    fi

    if [ "$BUILD_VERBOSE" = "ON" ]; then
        echo "=== OS information (detect_os_info) ==="
        echo "Detected OS[OS]:            $OS"
        echo "Distribution[DISTRO]:       $DISTRO"
        echo "Version[DISTRO_VERSION]:    $DISTRO_VERSION"
        echo "Package tool[PACKAGE_TOOL]: $PACKAGE_TOOL"
        echo "Architecture[ARCHITECTURE]: $ARCHITECTURE"
        echo "======================================="
        echo "PATH: $PATH"
    fi
}

# Simple version comparison function
version_compare() {
    local v1="$1"
    local v2="$2"

    if [[ "$v1" == "$v2" ]]; then
        return 0  # Equal
    fi

    local IFS=.
    local i v1_arr=($v1) v2_arr=($v2)

    for ((i=0; i<${#v1_arr[@]} || i<${#v2_arr[@]}; i++)); do
        local c1=$(printf "%010d" ${v1_arr[i]:-0})
        local c2=$(printf "%010d" ${v2_arr[i]:-0})

        if [[ "$c1" < "$c2" ]]; then
            return 1  # v1 < v2
        elif [[ "$c1" > "$c2" ]]; then
            return 0  # v1 > v2
        fi
    done

    return 0  # Equal
}

# Function to check if git is installed
check_git() {
    if ! command -v git >/dev/null 2>&1; then
        echo_color_error "X Git is not installed" >&2
        echo "  Git is required for this script to work." >&2
        echo "" >&2
        echo "  Please install Git using one of the following commands:" >&2

        # Detect OS and suggest installation command
        if [ -f /etc/debian_version ]; then
            echo "   Debian/Ubuntu: sudo apt update && sudo apt install -y git" >&2
        elif [ -f /etc/redhat-release ]; then
            echo "   RHEL/CentOS: sudo yum install -y git" >&2
            echo "   or" >&2
            echo "   Fedora: sudo dnf install -y git" >&2
        elif [ -f /etc/arch-release ]; then
            echo "   Arch Linux: sudo pacman -S git" >&2
        elif [ -f /etc/alpine-release ]; then
            echo "   Alpine Linux: sudo apk add git" >&2
        elif [[ "$OSTYPE" == "darwin"* ]]; then
            echo "   macOS (Homebrew): brew install git" >&2
            echo "   or download from: https://git-scm.com/download/mac" >&2
        else
            echo "   Please install Git from: https://git-scm.com/downloads" >&2
        fi

        return 1
    fi

    if [ "$1" = "1" ]; then

        # Optional: Check git version if minimum version required
        local min_version="${1:-2.0.0}"
        local git_version=$(git --version | cut -d' ' -f3)

        if ! version_compare "$git_version" "$min_version"; then
            echo_color_warn "!️ Git version $git_version is installed, but version $min_version or higher is recommended" >&2
            # Continue anyway, just a warning
        else
            echo_color_success "√ Git $git_version is installed"
        fi
    fi

    return 0
}

# Option to automatically install git (with user confirmation)
install_git_if_missing() {
    if ! command -v git >/dev/null 2>&1; then
        echo_color_warn "!️ Git is required but not installed." >&2
        read -p "Would you like to install Git now? (y/n): " -n 1 -r
        echo
        if [[ $REPLY =~ ^[Yy]$ ]]; then
            # Detect OS and install
            if [ -f /etc/debian_version ]; then
                sudo apt update && sudo apt install -y git
            elif [ -f /etc/redhat-release ]; then
                sudo yum install -y git
            elif [ -f /etc/arch-release ]; then
                sudo pacman -S --noconfirm git
            elif [ -f /etc/alpine-release ]; then
                sudo apk add git
            elif [[ "$OSTYPE" == "darwin"* ]]; then
                if command -v brew >/dev/null 2>&1; then
                    brew install git
                else
                    echo_color_error "X Homebrew not found. Please install Git manually from https://git-scm.com/download/mac" >&2
                    return 1
                fi
            else
                echo_color_error "X Unsupported OS. Please install Git manually." >&2
                return 1
            fi

            # Verify installation
            if command -v git >/dev/null 2>&1; then
                echo_color_success "√ Git installed successfully!"
            else
                echo_color_error "X Git installation failed. Please install manually." >&2
                return 1
            fi
        else
            echo_color_error "X Git is required. Exiting." >&2
            return 1
        fi
    fi
    return 0
}

# Function to create sed-safe pattern
sed_safe_pattern() {
    local pattern="$1"
    # Escape special chars for sed BRE
    echo "$pattern" | sed 's/\([][\.*^$+?(){}|/]\)/\\\1/g'
}

# Test function for sed pattern
test_sed_pattern() {
    local test_string="$1"
    local expected="$2"

    echo "Testing: $test_string"

    # Test with -E flag
    if echo "$test_string" | sed -E "s/${VERSION_PATTERN}/REPLACED/g" | grep -q "REPLACED"; then
        echo_color_success "  √ -E flag: Matches"
    else
        echo_color_error "  X -E flag: No match"
    fi

    # Convert to BRE and test
    BRE_PATTERN=$(sed_safe_pattern "$VERSION_PATTERN")
    if echo "$test_string" | sed "s/${BRE_PATTERN}/REPLACED/g" | grep -q "REPLACED"; then
        echo_color_success "  √ BRE: Matches"
    else
        echo_color_error "  X BRE: No match"
    fi
}

# Official SemVer 2.0.0 pattern. See: https://semver.org/
SEMVER_PATTERN='v?(0|[1-9][0-9]*)\.(0|[1-9][0-9]*)\.(0|[1-9][0-9]*)(-((0|[1-9][0-9]*|[0-9]*[a-zA-Z-][0-9a-zA-Z-]*)(\.(0|[1-9][0-9]*|[0-9]*[a-zA-Z-][0-9a-zA-Z-]*))*))?(\+([0-9a-zA-Z-]+(\.[0-9a-zA-Z-]+)*))?'
# [rpm version](https://docs.fedoraproject.org/en-US/packaging-guidelines/Versioning/)
RPM_VERSION_PATTERN="[0-9]+\.[0-9]+\.[0-9]+[+._~^0-9A-Za-z]*"
# SemVer, git, deb and rpm version pattern
VERSION_PATTERN="v?[0-9]+\.[0-9]+\.[0-9]+([-+_~.^][0-9A-Za-z.-]*)?"

# 函数：版本号解析器，返回包含所有信息的数组
# $1: 版本字符串
# #2: 输出的版本数组。需要用 `local -a` 声明：例如 `local -a version_info` 或者 `local version_info=()`
# 使用：
#    ver="v1.0.0"
#    local -a result
#    version_parser "$ver" result
#    if [[ $? -eq 0 ]]; then
#        display_version_info "${result[@]}"
#    fi
version_parser() {
    local version=$1
    local -n version_data=$2 # 使用引用传递
    
    # 定义版本号模式
    local major minor patch pre build

    # 移除 v 前缀
    version=${version#v}
    
    # 正则表达式匹配语义化版本
    local semantic_pattern=${SEMVER_PATTERN}

    if [[ $version =~ $semantic_pattern ]]; then
        # 基本版本信息 [major, minor, patch, pre, build, 完整版本, 是否预发布, 是否包含构建]
        version_data=(
            "${BASH_REMATCH[1]}"           # major
            "${BASH_REMATCH[2]}"           # minor
            "${BASH_REMATCH[3]}"           # patch
            "${BASH_REMATCH[5]:-}"         # pre
            "${BASH_REMATCH[10]:-}"        # build
            "$version"                     # original
            $([[ -n "${BASH_REMATCH[5]}" ]] && echo "true" || echo "false")  # is_pre
            $([[ -n "${BASH_REMATCH[10]}" ]] && echo "true" || echo "false")  # has_build
        )
        return 0
    fi

    echo_error "ERROR: Unable to parse version number '$version'" >&2
    return 1
}

# 辅助函数：显示版本数组
display_version_info() {
    local -a data=($@)

    if [[ ${#data[@]} -ge 8 ]]; then
        cat << EOF
版本信息:
  原始版本: ${data[5]}
  主版本号: ${data[0]}
  次版本号: ${data[1]}
  修订号: ${data[2]}
  预发布标签: ${data[3]:-(无)}
  构建元数据: ${data[4]:-(无)}
  是否预发布: ${data[6]}
  是否包含构建: ${data[7]}
EOF
    else
        echo_error "数据不完整"
    fi
}

# 函数：返回版本号的关联数组
# $1: 版本字符串
# #2: 输出的版本关联数组。需要用 `local -A` 声明：例如 `local -A version_info`
parse_version_assoc() {
    local version=$1
    local -n version_array=$2  # 使用引用传递
    # 正则表达式匹配语义化版本
    local pattern=$SEMVER_PATTERN
    #local pattern='^([0-9]+)\.([0-9]+)\.([0-9]+)(-([a-zA-Z0-9\.]+))?(\+([a-zA-Z0-9\.]+))?$'
    
    # 移除 v 前缀
    version=${version#v}
    
    if [[ $version =~ $pattern ]]; then
        version_array[major]="${BASH_REMATCH[1]}"
        version_array[minor]="${BASH_REMATCH[2]}"
        version_array[patch]="${BASH_REMATCH[3]}"
        version_array[pre]="${BASH_REMATCH[5]:-}"  # 默认空字符串
        version_array[build]="${BASH_REMATCH[10]:-}"
        return 0
    else
        echo_error "ERROR: Unable to parse version number '$version'" >&2
        return 1
    fi
}

# 比较预发布版本
compare_pre_release() {
    local pre1=$1
    local pre2=$2
    
    IFS='.' read -ra parts1 <<< "$pre1"
    IFS='.' read -ra parts2 <<< "$pre2"
    
    local i=0
    while [[ $i -lt ${#parts1[@]} ]] && [[ $i -lt ${#parts2[@]} ]]; do
        # 判断是数字还是字符串
        if [[ ${parts1[$i]} =~ ^[0-9]+$ ]] && [[ ${parts2[$i]} =~ ^[0-9]+$ ]]; then
            # 数字比较
            if [[ ${parts1[$i]} -gt ${parts2[$i]} ]]; then
                return 1
            elif [[ ${parts1[$i]} -lt ${parts2[$i]} ]]; then
                return 2
            fi
        elif [[ ${parts1[$i]} =~ ^[0-9]+$ ]] && [[ ! ${parts2[$i]} =~ ^[0-9]+$ ]]; then
            return 2  # 数字 < 非数字
        elif [[ ! ${parts1[$i]} =~ ^[0-9]+$ ]] && [[ ${parts2[$i]} =~ ^[0-9]+$ ]]; then
            return 1  # 非数字 > 数字
        else
            # 字符串比较
            if [[ ${parts1[$i]} > ${parts2[$i]} ]]; then
                return 1
            elif [[ ${parts1[$i]} < ${parts2[$i]} ]]; then
                return 2
            fi
        fi
        ((i++))
    done
    
    # 如果所有相同部分都相等，较长的预发布版本更高
    if [[ ${#parts1[@]} -gt ${#parts2[@]} ]]; then
        return 1
    elif [[ ${#parts1[@]} -lt ${#parts2[@]} ]]; then
        return 2
    fi
    
    return 0
}

# 版本号比较函数
# 返回值：
#   0: 版本相等
#   1: 版本1 > 版本2
#   2: 版本1 < 版本2
#   3: 版本格式错误
# Official SemVer 2.0.0 pattern. See: https://semver.org/
compare_versions() {
    local ver1=$1
    local ver2=$2

    # 解析版本号到数组
    local -a parts1
    local -a parts2

    version_parser "$ver1" parts1 || return 3
    version_parser "$ver2" parts2 || return 3
    #echo "parts1 size: ${#parts1[@]}; ${parts1[@]}"
    #echo "parts2 size: ${#parts2[@]}; ${parts2[@]}"

    # 检查数组元素个数
    if [ ${#parts1[@]} -lt 3 ] || [ ${#parts2[@]} -lt 3 ]; then
        echo_error "version format error" >&2
        return 3
    fi

    # 比较主版本、次版本、修订号
    for i in 0 1 2; do
        if [[ ${parts1[$i]} -gt ${parts2[$i]} ]]; then
            return 1
        elif [[ ${parts1[$i]} -lt ${parts2[$i]} ]]; then
            return 2
        fi
    done

    # 比较预发布版本（如果有）
    local pre1="${parts1[3]}"
    local pre2="${parts2[3]}"

    # 如果一个有预发布，一个没有，则有预发布的版本更低
    if [[ -n "$pre1" && -z "$pre2" ]]; then
        return 2
    elif [[ -z "$pre1" && -n "$pre2" ]]; then
        return 1
    elif [[ -n "$pre1" && -n "$pre2" ]]; then
        # 比较预发布版本
        compare_pre_release "$pre1" "$pre2"
        local result=$?
        [[ $result -ne 0 ]] && return $result
    fi

    return 0  # 完全相等
}

# 辅助函数：将返回值转换为可读文本
get_comparison_text() {
    case $1 in
        0) echo "相等" ;;
        1) echo "大于" ;;
        2) echo "小于" ;;
        3) echo "格式错误" ;;
        *) echo "未知($1)" ;;
    esac
}

test_version() {
    # 批量测试
    test_versions=(
        "1.2.3"
        "2.1.0-beta.2"
        "3.0.0+build.123"
        "v4.5.6-alpha.1+build.789"
        "5.6"
        "v5.6.6"
    )
    
    for ver in "${test_versions[@]}"; do
        echo "================================="
        echo "测试版本: $ver"
        local -a result #声明普通数组
        if version_parser "$ver" result; then
            display_version_info "${result[@]}"
        fi
    done
    
    # 使用示例
    local -A version_info #声明关联数组
    if parse_version_assoc "2.1.0-beta.2+build.456" version_info; then
        echo "主版本: ${version_info[major]}"
        echo "次版本: ${version_info[minor]}"
        echo "修订号: ${version_info[patch]}"
        echo "预发布: ${version_info[pre]}"
        echo "构建号: ${version_info[build]}"
    fi

    echo "compare_versions \"v1.0.0\" \"1.0.0\": `compare_versions "v1.0.0" "1.0.0"; echo $?`"
    echo "compare_versions \"v1.0.0\" \"1.0.0\": `compare_versions "2.0.0" "2.0.0"; echo $?`"
    echo "compare_versions \"v1.0.0\" \"1.1.0\": `compare_versions "v1.0.0" "1.1.0"; echo $?`"
    echo "compare_versions \"v2.0.0\" \"1.1.0\": `compare_versions "v2.0.0" "1.1.0"; echo $?`"
    echo "compare_versions \"v2.0.0\" \"2.0.0-alpha\": `compare_versions "v2.0.0" "2.0.0-alpha"; echo $?`"
    echo "compare_versions \"v2.0.0-alpha\" \"2.0.0-beta\": `compare_versions "v2.0.0-alpha" "2.0.0-beta"; echo $?`"
    echo "compare_versions \"1.0.0-alpha.1\" \"1.0.0-alpha.beta\": `compare_versions "1.0.0-alpha.1" "1.0.0-alpha.beta"; echo $?`"
    echo "compare_versions \"1.0.0-alpha\" \"1.0.0-alpha.1\": `compare_versions "1.0.0-alpha" "1.0.0-alpha.1"; echo $?`"
    echo "compare_versions \"1.0.0-rc.1\" \"1.1.0\": `compare_versions "1.0.0-rc.1" "1.1.0"; echo $?`"
    echo "compare_versions \"v2.0.0+dev\" \"v2.0.0+dev.1\": `compare_versions "v2.0.0+dev" "v2.0.0+dev.1"; echo $?`"
    echo "compare_versions \"v2.0.0-alpha+dev\" \"v2.0.0-alpha+dev.1\": `compare_versions "v2.0.0-alpha+dev" "v2.0.0-alpha+dev.1"; echo $?`"
}

# 获取第一个 ### 到第二个 ### 之间的内容（包括标记行）
# sed -n '/###/,/###/p' 文件
# 获取第一个 ### 到第二个 ### 之间的内容（不包括标记行）
# sed -n '/###/,/###/{//!p}' 文件
# 获取特定范围的 ### 之间的内容（例如第1对和第2对）
# sed -n '/###/,/###/p' 文件 | head -n -1  # 不包括结束标记
# 获取从第一个 ### 到最后一个 ### 之间的所有内容
# sed -n '/###/,/###/p' 文件

# 获取两个 ### 之间的内容（包括标记）
#awk '/###/,/###/' 文件
# 获取两个 ### 之间的内容（不包括标记）
#awk '/###/{flag=!flag; next} flag' 文件
# 获取第一个 ### 到第二个 ### 之间的内容（不包括标记）
# awk '/###/{count++; if(count==1){flag=1; next} if(count==2){flag=0; exit}} flag' 文件

#关键说明
#sed -n '/start/,/end/p'：打印从 start 到 end 的所有行
#awk '/start/,/end/'：打印从 start 到 end 的所有行
#{//!p}：sed 中的排除模式行，不打印匹配的行
#flag=!flag：awk 中的切换标志技巧
#next：跳过当前行，不打印
#选择哪种方法取决于：
#是否需要排除标记行
#是否需要处理多个区块
#文件大小（大文件建议用 awk）
#是否包含特殊字符

# 函数：获取两个标记之间的内容
# 参数1: 文件名
# 参数2: 标记（默认 ###）
# 参数3: 区块索引（默认 1，获取第几个区块）
get_section() {
    local file="$1"
    local marker="${2:-###}"
    local index="${3:-1}"

    awk -v marker="$marker" -v idx="$index" '
    $0 ~ marker {
        count++
        if (count == idx * 2 - 1) {
            in_section = 1
            next
        }
        if (count == idx * 2) {
            in_section = 0
            exit
        }
    }
    in_section {
        print
    }
    ' "$file"
}

# TODO: 初始化，必须放在此文件最后
init_blobal() {
    if [ ! $INIT_GLOBAL_RABBIT ]; then
        echo_status "Init global ......"
        export INIT_GLOBAL_RABBIT=TRUE
        #check_echo_color
        check_echo_color_with_tput
        detect_os_info
    fi
}

init_blobal
