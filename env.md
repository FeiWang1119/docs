# ohmyzsh

```sh
# zsh
sudo apt install zsh 

# ohmyzsh
sh -c "$(curl -fsSL https://raw.githubusercontent.com/ohmyzsh/ohmyzsh/master/tools/install.sh)"

# zsh-autosuggestions
git clone https://github.com/zsh-users/zsh-autosuggestions ${ZSH_CUSTOM:-~/.oh-my-zsh/custom}/plugins/zsh-autosuggestions

# zsh-syntax-highlighting
git clone https://github.com/zsh-users/zsh-syntax-highlighting.git ${ZSH_CUSTOM:-~/.oh-my-zsh/custom}/plugins/zsh-syntax-highlighting

# zsh-completions
git clone https://github.com/zsh-users/zsh-completions ${ZSH_CUSTOM:-${ZSH:-~/.oh-my-zsh}/custom}/plugins/zsh-completions

# fzf-tab
git clone https://github.com/Aloxaf/fzf-tab ${ZSH_CUSTOM:-~/.oh-my-zsh/custom}/plugins/fzf-tab

# Add it to FPATH in your .zshrc by adding the following line before source "$ZSH/oh-my-zsh.sh"
fpath+=${ZSH_CUSTOM:-${ZSH:-~/.oh-my-zsh}/custom}/plugins/zsh-completions/src

cp ~/repo/job-data/configs/.zshrc ~

# exa bat zoxide
sudo apt install exa bat zoxide

# fzf
git clone --depth 1 https://github.com/junegunn/fzf.git ~/.fzf
~/.fzf/install
```

# neovim

```sh
curl -LO https://github.com/neovim/neovim/releases/latest/download/nvim.appimage
chmod u+x nvim.appimage
./nvim.appimage

sudo mv ./nvim.appimage /usr/local/bin/nvim
```
Notice: clangd does work in MakeFile by using Bear tools

```sh
bear -- <your-build-command>
```

## install qml treesitter languages

TSInstall qmljs qmlldir


# lazygit

```sh
LAZYGIT_VERSION=$(curl -s "https://api.github.com/repos/jesseduffield/lazygit/releases/latest" | grep -Po '"tag_name": "v\K[^"]*')
curl -Lo lazygit.tar.gz "https://github.com/jesseduffield/lazygit/releases/latest/download/lazygit_${LAZYGIT_VERSION}_Linux_x86_64.tar.gz"
tar xf lazygit.tar.gz lazygit
sudo install lazygit /usr/local/bin
```

# LazyVim

```sh
git clone https://github.com/LazyVim/starter ~/.config/nvim
```

## dependencies

``` sh
# install ripgrep, or telescope grep doesn't work
apt install ripgrep 

# markdown-preview
apt install npm
cd ~/.local/share/nvim/lazy/markdown-preview.nvim/app && npm install

# previde a clipboard tool like xclip for using system's clipboard. 
apt install xclip 
```

## cmake

cmake add `CMAKE_EXPORT_COMPILE_COMMANDS=ON` to remove clangd warnings
