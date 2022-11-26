[project-page-generator](https://github.com/Kei18/project-page-generator)
===

Simple project-page generator for research, assuming hosted on GitHub Pages.

[[demo page]](https://kei18.github.io/project-page-generator/)

## How to Deploy

### 1. Clone this repo

```sh
git clone git@github.com:Kei18/project-page-generator.git project-page
cd project-page
```

### 2. Change remote repo

```sh
git remote set-url origin {YOUR GITHUB REPO}
```

### 3. Edit your project-page

All you need is [yarn](https://yarnpkg.com/).
Initialize the repo with the following script:

```sh
yarn
```

Edit `data.yaml`, then:

```sh
yarn serve
```

You can see the website via `http://localhost:1234`.


### 4. Push

```sh
git push origin project-page
```

### 5. Publish

- Go to `Your github repo -> Settings -> Pages`
- Choose `gh-pages` in `Branch` menu.

## Notes

Auto formatting (Prettier) when committing:

```sh
git config core.hooksPath .githooks && chmod a+x .githooks/pre-commit
```

## Licence

This software is released under the MIT License, see [LICENSE.txt](LICENCE.txt).

## Author

[Keisuke Okumura](https://kei18.github.io) is a Ph.D. student at Tokyo Institute of Technology, interested in controlling multiple moving agents.
