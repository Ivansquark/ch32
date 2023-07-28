syntax on
runtime! ftplugin/man.vim
"=============================== BASIC SETTINGS ==============================
"=============================================================================
filetype plugin indent on
highlight SpellBad ctermfg=Black ctermbg=Red
hi StatusLine gui=reverse cterm=reverse
set autoread
"set backup
set nobackup                    " Some servers have issues with backup files
set nowritebackup
set browsedir=current
set clipboard=unnamed
set encoding=utf-8
set hidden                      " TextEdit might fail if hidden is not set.
set history=128
set mouse=a
set nocompatible
set noswapfile
set sessionoptions=curdir,buffers,tabpages
"set t_Co=256
set termencoding=utf-8
set title
set undolevels=2048
set splitbelow                  "устанавливаем всплывающее окно снизу (терминал)
set termwinsize=10x0
"=============================================================================
"========================== NAVIGATION SETTINGS ==============================
"=============================================================================
"togle between header and source in one directory
"nnoremap <F3> :e %:p:s,.h$,.X123X,:s,.cpp$,.h,:s,.X123X$,.cpp,<CR>
"":term to bottom
set splitbelow
set termwinsize=10x0 " 'termsize' didn't work - this did for me (GVIM 8.2)
set splitright
" split navigations
nnoremap <C-J> <C-W><C-J>
nnoremap <C-K> <C-W><C-K>
nnoremap <C-L> <C-W><C-L>
nnoremap <C-H> <C-W><C-H>
" Ctrl-Left or Ctrl-Right to go to the previous or next tabs
nnoremap <C-Left> :tabprevious<CR>
nnoremap <C-Right> :tabnext<CR>
" Alt-Left or Alt-Right to move the current tab to the left or right
nnoremap <silent> <C-Down> :execute 'silent! tabmove ' . (tabpagenr()-2)<CR>
nnoremap <silent> <C-Up> :execute 'silent! tabmove ' . (tabpagenr()+1)<CR>
" buffers:::::: bn, bp, bd-delete, bw - realy delete
"=============================================================================
"============================== EDITOR SETTINGS ==============================
"=============================================================================
autocmd CursorMoved * silent! exe printf("match Search /\\/", expand(''))
let c_syntax_for_h=""
let python_highlight_all = 1
set backspace=indent,eol,start
set laststatus=2
set linebreak

"set mps+=
set noruler
set number                      " показывать номера строк
set scrolloff=4
set showcmd
set showmatch
set cursorline                  " Подсвечивать строку с курсором
set textwidth=80               " ширина текста
let &colorcolumn=&textwidth     " Подсвечивать 120 стобец
highlight ColorColumn ctermbg=darkgray
"set visualbell                 " Раздражающая подсветка
set wrap
set noequalalways               " Не делать все окна одинакового размера
set winheight=20                " Высота окон по-умолчанию 20 строк
syntax on                       " Подсвечивать синтаксис
set ttimeoutlen=1 "Понижаем задержку ввода escape последовательностей
let &t_SI.="\e[5 q" "SI = режим вставки Где 1 - это мигающий прямоугольник 2 - обычный прямоугольник
let &t_SR.="\e[3 q" "SR = режим замены   3 - мигающее подчёркивание  4 - просто подчёркивание
let &t_EI.="\e[1 q" "EI = нормальный режим  5 - мигающая вертикальная черта  6 - просто вертикальная черта

inoremap <expr> <CR>
    \ getline('.')[col('.')-1]=='}'
    \ ? "\<CR>\<C-d>\<C-o>O" : "\<CR>"
"=============================================================================
"============================ INDENTATION SETTINGS ===========================
"=============================================================================
set expandtab       " Заменять табуляцию на пробелы
set shiftwidth=4    " size of an indent
set smarttab
set smartindent     " автоматически расставлять отступы
"set softtabstop=4
set tabstop=4       " задать размер табуляции в четыре пробела
set cindent
au FileType crontab,fstab set noexpandtab tabstop=8 shiftwidth=8
au FileType make set noexpandtab tabstop=4 shiftwidth=4
au FileType *.htm,*.html,*.css,*.js set softtabstop=2 tabstop=2 shiftwidth=2 textwidth=120
"=============================================================================
"============================= TEXT SEARCH SETTINGS ==========================
"=============================================================================
set ic      " Игнорировать регистр при поиске
set hls     " Подсвечивать поиск
set is      " Использовать инкреминтированный поиск
set wildmode=longest:list,full      "bash-подобное дополнение командной строки
"=============================================================================
"================================= COLORSCHEME ===============================
"=============================================================================
set rtp+=~/.vim/bundle/vim-code-dark
colorscheme codedark
let g:airline_theme = 'codedark'
"=============================================================================
"================================= PLUGINS ===================================
"=============================================================================
" Vundle required config
set nocompatible              " be iMproved, required
filetype off                  " required
 
" set the runtime path to include Vundle and initialize
set rtp+=~/.vim/bundle/Vundle.vim
call vundle#begin()
" " let Vundle manage Vundle, required
" User defined plugins
Plugin 'gmarik/Vundle.vim'
"Plugin 'Valloric/YouCompleteMe'
Plugin 'preservim/nerdtree'
Plugin 'tomasiser/vim-code-dark'
Plugin 'vim-airline/vim-airline'
Plugin 'neoclide/coc.nvim'
Plugin 'vim-syntastic/syntastic'
Plugin 'rhysd/vim-clang-format'
Plugin 'jackguo380/vim-lsp-cxx-highlight'
Plugin 'jiangmiao/auto-pairs'
Plugin 'octol/vim-cpp-enhanced-highlight'
"Plugin 'puremourning/vimspector'
" " All of your Plugins must be added before the following line
call vundle#end()            " required

"============================================================================
"============================ PLUGINS SETTINGS ==============================
"============================================================================
:filetype on 
"set runtimepath+=~/.vim/bundle/YouCompleteMe/
"let g:ycm_global_ycm_extra_conf = '~/.vim/bundle/YouCompleteMe/.ycm_extra_conf.py'
"let g:ycm_seed_identifiers_with_syntax=1
"let g:ycm_confirm_extra_conf=0
"let g:ycm_collect_identifiers_from_tag_files = 1
"set completeopt=longest,menu
"------------------ NERDTree --------------------
"autocmd vimenter * NERDTree
:let g:NERDTreeWinSize=20
"let NERDTreeIgnore=['\.pyc$', '\~$']
let NERDTreeMapOpenInTab='\r'
let NERDTreeShowHidden=1
" Configure Nerdtree toggle
map <C-n> :NERDTreeToggle<CR>
"---------------    AIRLINE     -----------------
let g:airline_powerline_fonts = 1
if !exists('g:airline_symbols')
  let g:airline_symbols = {}
endif
"  " powerline symbols
let g:airline_left_sep = ''
let g:airline_left_alt_sep = ''
let g:airline_right_sep = ''
let g:airline_right_alt_sep = ''
let g:airline_symbols.branch = ''
let g:airline_symbols.colnr = ' '
let g:airline_symbols.readonly = ''
let g:airline_symbols.linenr = ' '
let g:airline_symbols.maxlinenr = ' '
let g:airline_symbols.dirty='⚡'
"---------    C++ syntax highlight  ------------------
let g:cpp_class_scope_highlight = 1
let g:cpp_member_variable_highlight = 1
let g:cpp_class_decl_highlight = 1
let g:cpp_posix_standard = 1
let g:cpp_concepts_highlight = 1
"---------    syntastic syntax checker  --------------
let g:syntastic_cpp_compiler = 'gcc'
let g:syntastic_cpp_compiler_options = ' -std=c++17 -stdlib=libc++'
"------     Auto-Formatting     ------------------
".clang-format config-file in the root of your project.
"+--------------------------------------------------+
"|                  CoC nvim                        |
"+--------------------------------------------------+
set cmdheight=2     " Give more space for displaying messages.
" Having longer updatetime (default is 4000 ms = 4 s) leads to noticeable
" delays and poor user experience.
set updatetime=300
nmap <silent> [c <Plug>(coc-diagnostic-prev)
" Use `[c` and `]c` to navigate diagnostics
nmap <silent> ]c <Plug>(coc-diagnostic-next)
" Use <c-space> to trigger completion.
inoremap <silent><expr> <c-space> coc#refresh()
" Use tab for trigger completion with characters ahead and navigate.
" NOTE: Use command ':verbose imap <tab>' to make sure tab is not mapped by
" other plugin before putting this into your config.
inoremap <silent><expr> <C-TAB>
      \ pumvisible() ? "\<C-n>" :
      \ <SID>check_back_space() ? "\<TAB>" :
      \ coc#refresh()
inoremap <expr><S-TAB> pumvisible() ? "\<C-p>" : "\<C-h>"
" Use <c-space> to trigger completion.
if has('nvim')
  inoremap <silent><expr> <c-space> coc#refresh()
else
  inoremap <silent><expr> <c-@> coc#refresh()
endif
" Make <CR> auto-select the first completion item and notify coc.nvim to
" format on enter, <cr> could be remapped by other vim plugin
inoremap <silent><expr> <CR> coc#pum#visible() ? coc#pum#confirm() : "\<CR>"
" Use K to show documentation in preview window.
nnoremap <silent> K :call <SID>show_documentation()<CR>
function! s:show_documentation()
  if (index(['vim','help'], &filetype) >= 0)
    execute 'h '.expand('<cword>')
  elseif (coc#rpc#ready())
    call CocActionAsync('doHover')
  else
    execute '!' . &keywordprg . " " . expand('<cword>')
  endif
endfunction
" Formatting selected code.
nmap <F6>  :call CocAction('format')<CR>   
" Formatting selected. Not sure if these work...
xmap <F5>  <Plug>(coc-format-selected)
nmap <F5>  <Plug>(coc-format-selected)

augroup mygroup
  autocmd!
  " Setup formatexpr specified filetype(s).
  autocmd FileType typescript,json setl formatexpr=CocAction('formatSelected')
  " Update signature help on jump placeholder.
  autocmd User CocJumpPlaceholder call CocActionAsync('showSignatureHelp')
augroup end

" Add (Neo)Vim's native statusline support.
" NOTE: Please see `:h coc-status` for integrations with external plugins that
" provide custom statusline: lightline.vim, vim-airline.
set statusline^=%{coc#status()}%{get(b:,'coc_current_function','')}

function! s:check_back_space() abort
  let col = col('.') - 1
  return !col || getline('.')[col - 1]  =~# '\s'
endfunction
" always show signcolumns
set signcolumn=yes
" key mappings example
nmap <silent> <F12> <Plug>(coc-definition)
nmap <silent> gD <Plug>(coc-implementation)
nmap <silent> <F10> <Plug>(coc-references)
function! CloseAllBuffersButCurrent()
    let l:curr = bufnr('%')
    let l:last = bufnr('$')
    if l:curr != 1 | silent! execute (l:curr).'bd' | endif
endfunction
nmap <F9> :call CloseAllBuffersButCurrent()<CR>

nnoremap <Leader>f :<C-u>ClangFormat<CR>

"----- folding ----------------------------------------------------------------
set foldmethod=syntax
set foldnestmax=1
