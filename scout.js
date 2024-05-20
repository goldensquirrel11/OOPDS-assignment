#!/usr/bin/env node
const { spawnSync, execFileSync } = require("child_process");
const fs = require("fs");
const spawn = require("child_process");
const args = process.argv.slice(2);

const app = {
  /**
   * Checks if a filename was passed as command
   * line arguments.
   */
  init: function () {
    that = this;
    if (args[0] === undefined || args[0] === "") {
      console.error("Error: No filename passed, exiting...");
      return;
    }
    const file_name = "./" + args[0];
    that.watch_file(file_name);
  },
  /**
   * Watches .cpp file for changes.
   */
  watch_file: function (file_name) {
    let fswait = false;
    console.log(
      `\x1b[32m[Scout] Watching process started for file ${file_name}\x1b[0m`
    );
    let ext = args[0].length - 4;
    let output_file = args[0].substring(0, ext);
    fs.watch(file_name, (event, filename) => {
      if (filename) {
        if (fswait) return;
        fswait = setTimeout(() => {
          fswait = false;
        }, 1500);
        console.log(`\x1b[32m[Scout] change detected in ${file_name}\x1b[0m`);
        const compile_file = spawnSync("g++", ["-o", output_file, file_name], {
          encoding: "utf-8",
        });
        const run_file = spawnSync(`${output_file}`, { encoding: "utf-8" });

        console.log(`\x1b[32m[Scout] Compiled ${file_name}\x1b[0m`);
        if (compile_file.stdout !== "" || compile_file.stderr !== "") {
          console.log(`\x1b[33m[Scout] Compiler output detected\x1b[0m`);
          if (compile_file.stdout !== "") console.log(`${compile_file.stdout}`);
          if (compile_file.stderr !== "") console.log(`${compile_file.stderr}`);
        }

        console.log(`\x1b[36m[Scout] Running ${file_name}\x1b[0m`);
        if (run_file.stdout !== "") console.log(run_file.stdout);
        if (run_file.stderr !== "") console.log(run_file.stderr);
      }
    });
  },
};
//Starts the application
app.init();

module.exports = app;
