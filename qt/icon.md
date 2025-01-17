# Icons and themes are looked for in a set of directories.

 - $HOME/.icons (for backwards compatibility)
 - $XDG_DATA_DIRS/icons 
 - /usr/share/pixmaps 
 - apps add own icon directories

# icon theme

From a programmer perspective an icon theme is just a mapping. 
Given a set of directories to look for icons in and a theme name it maps from icon name and nominal icon size to an icon filename.

A theme can be spread across several base directories by having subdirectories of the same name. This way users can extend and override system themes.

In at least one of the theme directories there must be a file called index.theme that describes the theme.
The first index.theme found while searching the base directories in order is used. This file describes the general attributes of the theme.

# index.theme

```
[Icon Theme]
Name=Birch
Name[sv]=Björk
Comment=Icon theme with a wooden look
Comment[sv]=Träinspirerat ikontema
Inherits=wood,default
Directories=48x48/apps,48x48@2/apps48x48/mimetypes,32x32/apps,32x32@2/apps,scalable/apps,scalable/mimetypes

[scalable/apps]
Size=48
Type=Scalable
MinSize=1
MaxSize=256
Context=Applications

[scalable/mimetypes]
Size=48
Type=Scalable
MinSize=1
MaxSize=256
Context=MimeTypes

[32x32/apps]
Size=32
Type=Fixed
Context=Applications

[32x32@2/apps]
Size=32
Scale=2
Type=Fixed
Context=Applications

[48x48/apps]
Size=48
Type=Fixed
Context=Applications

[48x48@2/apps]
Size=48
Scale=2
Type=Fixed
Context=Applications

[48x48/mimetypes]
Size=48
Type=Fixed
Context=MimeTypes
```
# fromTheme

To make QIcon use the high DPI version of an image, add an additional entry to the appropriate index.theme file:

```
[Icon Theme]
Name=Test
Comment=Test Theme

Directories=32x32/actions,32x32@2/actions

[32x32/actions]
Size=32
Context=Actions
Type=Fixed

# High DPI version of the entry above.
[32x32@2/actions]
Size=32
Scale=2
Type=Fixed
```

Your icon theme directory would then look something like this:

```
├── 32x32
│   └── actions
│       └── appointment-new.png
├── 32x32@2
│   └── actions
│       └── appointment-new.png
└── index.theme
```

To fetch an icon from the current icon theme:

```c++
QIcon undoicon = QIcon::fromTheme("edit-undo");
```

# icon lookup

The icon lookup mechanism has two global settings, the list of base directories and the internal name of the current theme.
Given these we need to specify how to look up an icon file from the icon name, the nominal size and the scale.

The lookup is done first in the current theme, and then recursively in each of the current theme's parents, 
and finally in the default theme called "hicolor" (implementations may add more default themes before "hicolor", but "hicolor" must be last).
As soon as there is an icon of any size that matches in a theme, the search is stopped. 
Even if there may be an icon with a size closer to the correct one in an inherited theme, we don't want to use it. 
Doing so may generate an inconsistant change in an icon when you change icon sizes (e.g. zoom in).

The lookup inside a theme is done in three phases. 

1. First all the directories are scanned for an exact match, e.g. one where the allowed size of the icon files match what was looked up.
2. Then all the directories are scanned for any icon that matches the name. 
3. If that fails we finally fall back on unthemed icons. 

If we fail to find any icon at all it is up to the application to pick a good fallback, as the correct choice depends on the context.
