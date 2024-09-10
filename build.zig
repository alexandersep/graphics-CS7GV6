const std = @import("std");

pub fn build(b: *std.Build) !void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const exe = b.addExecutable(.{
        .name = "graphics",
        .target = target,
        .optimize = optimize,
    });

    const lib_dir = "lib/";
    const glfw_dir = lib_dir ++ "glfw-3.4/";

    exe.linkSystemLibrary("glfw3");
    exe.linkSystemLibrary("GL");
    exe.addLibraryPath(b.path(lib_dir));
    exe.addIncludePath(b.path("include"));
    exe.addCSourceFiles(.{ .files = &generic_source_files });
    exe.linkLibC();

    exe.addIncludePath(b.path(glfw_dir ++ "include"));

    b.installArtifact(exe);

    const run_cmd = b.addRunArtifact(exe);
    run_cmd.step.dependOn(b.getInstallStep());
    if (b.args) |args| {
        run_cmd.addArgs(args);
    }

    const run_step = b.step("run", "Run the app");
    run_step.dependOn(&run_cmd.step);
}

const generic_source_files = [_][]const u8{
    "src/stb_image.c",
    "src/gl.c",
    "src/main.c",
    "src/control.c",
};
