# Guide: Testing PATH Directory Order in Minishell
This document explains how to test whether a minishell implementation correctly searches for executables in PATH directories in left-to-right order.

## Test Setup (Already in Repository)
The following test environment already exists in the repository:
1. Test directory structure:
   ```
   docs/dir1
   docs/dir2
   ```

2. Test executable in the first directory:
   ```
   docs/dir1/test (executable, outputs "This is from dir1")
   ```

3. Test executable with the same name in the second directory:
   ```
   docs/dir2/test (executable, outputs "This is from dir2")
   ```

4. Verify both scripts work correctly by running them directly:
   ```bash
   ./docs/dir1/test    # Should print "This is from dir1"
   ./docs/dir2/test    # Should print "This is from dir2"
   ```

## Testing in Minishell
1. Start minishell implementation:
   ```bash
   ./minishell
   ```

2. Set the PATH environment variable with dir1 before dir2:
   ```bash
   export PATH=/full_path_to/docs/dir1:/full_path_to/docs/dir2:$PATH
   ```
   (Replace `/full_path_to/` with the actual path, e.g., `$(pwd)/`)

3. Verify the PATH is set correctly:
   ```bash
   echo $PATH
   ```

4. Run the test command:
   ```bash
   test
   ```
   If PATH ordering is correct, this should execute dir1/test and print "This is from dir1"

5. Reverse the order to verify behavior changes:
   ```bash
   export PATH=/full_path_to/docs/dir2:/full_path_to/docs/dir1:$PATH
   test
   ```
   This should now print "This is from dir2"

## Troubleshooting
If you get "command not found":
- Ensure the paths in PATH are absolute, not relative
- Check that the directories exist and are accessible
- Verify the command name doesn't conflict with built-ins

If you get "Exec format error":
- Ensure the script has the correct line endings (LF, not CRLF)
- Verify the shebang interpreter exists (#!/bin/bash)
- Check file permissions are correct (chmod +x)

If the wrong script executes:
- Use `which test` to see which path is being found
- Check if a system command with the same name exists
- Try with a unique command name that doesn't conflict with system commands

## Implementation Notes
For minishell implementation, ensure:
1. PATH is split by colons into an array of directories
2. Directories are searched in order from left to right
3. The first executable found with the requested name is executed
4. If no executable is found in any directory, "command not found" is returned