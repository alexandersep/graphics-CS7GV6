const std = @import("std");

pub fn build(b: *std.Build) !void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});
    const t = target.result;

    const exe = b.addExecutable(.{
        .name = "graphics",
        .target = target,
        .optimize = optimize,
    });

    if (t.os.tag == std.Target.Os.Tag.windows) {
        exe.linkSystemLibrary("opengl32");
        exe.linkSystemLibrary("c");
        exe.linkSystemLibrary("user32");
        exe.linkSystemLibrary("gdi32");
        exe.linkSystemLibrary("shell32");

        exe.addLibraryPath(.{ .cwd_relative = "C:\\Program Files (x86)\\GLFW\\lib" });
        exe.addIncludePath(.{ .cwd_relative = "C:\\Program Files (x86)\\GLFW\\include" });
    } else { // linux
        exe.linkSystemLibrary("GL");

        exe.addLibraryPath(.{ .cwd_relative = "/home/alex/.local/lib" });
        exe.addIncludePath(.{ .cwd_relative = "/home/alex/.local/include" });
    }
    exe.linkSystemLibrary2("glfw3", .{ .preferred_link_mode = .static });
    exe.linkSystemLibrary2("cglm", .{ .preferred_link_mode = .static });
    exe.linkSystemLibrary("assimp"); // dynamic build
    exe.addIncludePath(b.path("include"));
    exe.addCSourceFiles(.{ .files = &generic_source_files });
    exe.linkLibC();

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
    "src/single_headers.c",
    "src/glad.c",
    "src/main.c",
    "src/control.c",
    "src/shader.c",
    "src/file.c",
    "src/texture.c",
    "src/mesh.c",
    "src/tool.c",
    "src/model.c",
    "src/mantaray.c",
    "src/boids.c",
    "src/fish.c",
    "src/cubemap.c",
    "src/framebuffer.c",
    "src/renderbuffer.c",
    "src/screen.c",
    "src/transparent.c",
};
