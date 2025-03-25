# SVG Parser Command Documentation

## Implementation Overview
This SVG parser uses the **Singleton Factory Pattern** for figure creation. 
When adding a new figure type, developers must implement:
1. A new figure class inheriting from base `Figure` class
2. A creator class inheriting from `FigureUserCreator` (for user input creation)
3. A creator class inheriting from `FigureFileCreator` (for file parsing creation)

## Performance Characteristics
- Adding a figure: O(1)
- Removing a figure: O(n) (where n = number of figures)
- Transformations: O(logN)
- Rendering: O(n)

## File Operations
- `open <file>` - Opens an SVG file
- `save` - Saves all figures to the current file
- `saveas <file>` - Saves all figures to a specified file

## Display Commands
- `print` - Displays all figures in the console
- `help` - Shows available commands

## Figure Creation

General creation:
- `create <figure> <components>` - Creates a new figure with specified components

### Basic Figures:
- `rectangle <x> <y> <width> <height> <stroke> <fill>`
- `arc <cx> <cy> <r> <startingAngle> <lengthAngle> <stroke> <fill>`
- `polygon <countPoints> [<x> <y>] <stroke> <fill>`
- `line <x1> <y1> <x2> <y2> <stroke>`


## Transformation Commands
- `translate <id> vertical=<v> horizontal=<h>` - Moves a specific figure
- `translate vertical=<v> horizontal=<h>` - Moves all figures
- `scale <id> <width> <height>` - Resizes a specific figure
- `scale <width> <height>` - Resizes all figures

## Group Management
- `group <x1> <y1> <x2> <y2>` - Groups all figures within specified region
- `ungroup <groupId>` - Deletes a group (keeps elements)
- `insert <groupId> <elemId>` - Adds a figure to a group
- `extract <groupId> <elemId>` - Removes a figure from a group

## Visibility Control
- `hide <id>` - Makes figure invisible (not printed/saved)
- `show <id>` - Restores visibility of hidden figure

### Notes:
- `<file>`: Path to SVG file
- `<id>`: Figure identifier
- `<stroke>`: Outline color
- `<fill>`: Fill color
- All coordinates and dimensions are in pixels
- Angles are specified in degrees
- For polygons: `<countPoints>` specifies vertex count, followed by <x> <y> pairs