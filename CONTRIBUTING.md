# TagTinker-Enhanced

Enhanced version of [i12bp8/TagTinker](https://github.com/i12bp8/TagTinker).  
**Author (original app):** i12bp8 · **TagTinker EE:** GlitchDaBest

## Build & install

1. Clone this repository:
   ```bash
   git clone https://github.com/GlitchDaBest/TagTinker-Enhanced.git
   cd TagTinker-Enhanced
   ```
2. Install [ufbt](https://github.com/flipperdevices/flipperzero-ufbt) and build:
   ```bash
   ufbt
   ufbt launch
   ```
3. On first run, the app creates `apps_data/tagtinker/dropped/` on the SD card.
4. Prepare BMPs with [Image Prep](https://i12bp8.github.io/TagTinker/) (or `web-image-prep/index.html` locally).

Test on real hardware when you can; at minimum run `ufbt` and confirm the build passes before pushing changes.

## Contributing

All work happens on **`main`**.

1. Branch from `main`: `git checkout -b my-fix`
2. Make focused changes; keep commits clear
3. Open a pull request into **`main`** on **GlitchDaBest/TagTinker-Enhanced**

## Contributing upstream

To propose changes to the **original** TagTinker project, follow [i12bp8/TagTinker](https://github.com/i12bp8/TagTinker) and open a PR there (fork → feature branch → PR).

## Code style

- C99, no C++ features
- 4-space indentation (no tabs)
- `snake_case` for functions and variables
- Keep functions short and focused

## Pull request guidelines

- Use **"Create a merge commit"** when merging to preserve contributor attribution
- One feature or fix per PR when possible

## Reporting issues

Please open a GitHub issue with:

- What you expected vs what happened
- Flipper firmware version
- ESL model / type code (if applicable)

## License

Contributions are licensed under GPL-3.0, consistent with the upstream project.
