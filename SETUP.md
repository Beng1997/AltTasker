# AltTasker - Setup Instructions

## Quick Setup (Recommended Way)

### Step 1: Open WSL Terminal (Not PowerShell!)

```bash
# Open WSL directly
wsl
```

### Step 2: Navigate and Compile

```bash
cd /mnt/d/AltTasker
make clean && make
```

### Step 3: Install the 'task' command

```bash
chmod +x scripts/install-alias.sh
./scripts/install-alias.sh
source ~/.bashrc
```

### Step 4: Run it!

```bash
task
```

That's it! Now you can run `task` from anywhere in WSL.

---

## How to Use

- **Start**: Just type `task` in WSL
- **Exit**: Press `Ctrl+C`
- **Refresh**: Automatically every 2 seconds

---

## Important Notes

⚠️ **DO NOT run from PowerShell!**
- PowerShell doesn't properly support terminal control codes
- This causes the scrolling issue you saw
- Always run from **native WSL terminal**

✅ **How to open WSL terminal:**
1. Windows Terminal → Ubuntu/WSL tab
2. Or: Run `wsl` from Start menu
3. Or: Open Windows Terminal and select your Linux distro

---

## Troubleshooting

### "bash: task: command not found"
```bash
source ~/.bashrc
# Or restart your terminal
```

### Permission denied
```bash
chmod +x /mnt/d/AltTasker/alttasker
```

### Still see scrolling?
Make sure you're running from **WSL terminal**, not PowerShell!

---

## Advanced: Create System-Wide Command

If you want `task` to work system-wide:

```bash
sudo ln -s /mnt/d/AltTasker/alttasker /usr/local/bin/task
```

Now `task` works from any directory!

---

**Enjoy your Linux task monitor! 🚀**
