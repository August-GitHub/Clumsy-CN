const std = @import("std");
const Builder = std.build.Builder;
const Pkg = std.build.Pkg;
const Step = std.build.Step;
const debug = std.debug;
const Allocator = std.mem.Allocator;
const CrossTarget = std.zig.CrossTarget;

const ClumsyArch = enum { x86, x64 };
const ClumsyConf = enum { Debug, Release, Ship };
const ClumsyWinDivertSign = enum { A, B, C };

pub fn build(b: *std.build.Builder) void {
    const arch = b.option(ClumsyArch, "arch", "x86, x64") orelse .x64;
    const conf = b.option(ClumsyConf, "conf", "Debug, Release") orelse .Debug;
    const windivert_sign = b.option(ClumsyWinDivertSign, "sign", "A, B, C") orelse .A;
    const windows_kit_bin_root = b.option([]const u8, "windows_kit_bin_root", "Windows SDK Bin root") orelse "C:/Program Files (x86)/Windows Kits/10/bin/10.0.19041.0";

    const arch_tag = @tagName(arch);
    const conf_tag = @tagName(conf);
    const sign_tag = @tagName(windivert_sign);
    const windivert_dir = b.fmt("WinDivert-2.2.0-{s}", .{sign_tag});

    debug.print("- arch: {s}, conf: {s}, sign: {s}\n", .{@tagName(arch), @tagName(conf), @tagName(windivert_sign)});
    debug.print("- windows_kit_bin_root: {s}\n", .{windows_kit_bin_root});
    _ = std.fs.realpathAlloc(b.allocator, windows_kit_bin_root) catch @panic("windows_kit_bin_root not found");

    const prefix = b.fmt("{s}_{s}_{s}", .{arch_tag, conf_tag, sign_tag});
    const output_dir = b.fmt("{s}/{s}", .{b.install_path, prefix});

    debug.print("- out: {s}\n", .{output_dir});

    const tmp_path = b.fmt("tmp/{s}", .{prefix});
    std.fs.cwd().makePath(tmp_path) catch @panic("unable to create tmp directory");

    b.installFile(b.fmt("external/{s}/{s}/WinDivert.dll", .{windivert_dir, arch_tag}), b.fmt("{s}/WinDivert.dll", .{prefix}));
    switch (arch) {
        .x64 => b.installFile(b.fmt("external/{s}/{s}/WinDivert64.sys", .{windivert_dir, arch_tag}), b.fmt("{s}/WinDivert64.sys", .{prefix})),
        .x86 => b.installFile(b.fmt("external/{s}/{s}/WinDivert32.sys", .{windivert_dir, arch_tag}), b.fmt("{s}/WinDivert32.sys", .{prefix})),
    }

    b.installFile("etc/config.txt", b.fmt("{s}/config.txt", .{prefix}));
    if (conf == .Ship)
        b.installFile("LICENSE", b.fmt("{s}/License.txt", .{prefix}));

    const res_obj_path = b.fmt("{s}/clumsy_res.obj", .{tmp_path});

    const rc_exe = b.findProgram(&.{
        "rc.exe",
    }, &.{
        std.fs.path.join(b.allocator, &[2][]const u8{windows_kit_bin_root, @tagName(arch)}) catch unreachable,
    }) catch @panic("unable to find `rc.exe`, check your windows_kit_bin_root");

    const archFlag = switch (arch) {
        .x86 => "X86",
        .x64 => "X64",
    };
    const cmd = b.addSystemCommand(&.{
        rc_exe,
        "/nologo",
        "/d",
        "NDEBUG",
        "/d",
        archFlag,
        "/r",
        "/fo",
        res_obj_path,
        "etc/clumsy.rc",
    });

    // Zig 0.11.0: addExecutable 需要指定源文件或使用 addCSourceFile 添加
    const exe = b.addExecutable(.{
        .name = "clumsy",
        .root_source_file = null,
    });

    switch (conf) {
        .Debug => {
            exe.setOptimizeMode(.Debug);
            exe.subsystem = .Console;
        },
        .Release => {
            exe.setOptimizeMode(.ReleaseSafe);
            exe.subsystem = .Windows;
        },
        .Ship => {
            exe.setOptimizeMode(.ReleaseFast);
            exe.subsystem = .Windows;
        },
    }

    // Zig 0.11.0: 使用标准目标查询
    const target = b.standardTargetOptions(.{});
    exe.setTarget(target);

    exe.step.dependOn(&cmd.step);
    
    // Zig 0.11.0: addObjectFile 需要 LazyPath
    exe.addObjectFile(.{ .path = res_obj_path });
    
    // Zig 0.11.0: addCSourceFile 使用 .{ .file = ..., .flags = ... } 格式
    exe.addCSourceFile(.{ .file = .{ .path = "src/bandwidth.c" }, .flags = &.{} });
    exe.addCSourceFile(.{ .file = .{ .path = "src/divert.c" }, .flags = &.{} });
    exe.addCSourceFile(.{ .file = .{ .path = "src/drop.c" }, .flags = &.{} });
    exe.addCSourceFile(.{ .file = .{ .path = "src/duplicate.c" }, .flags = &.{} });
    exe.addCSourceFile(.{ .file = .{ .path = "src/elevate.c" }, .flags = &.{} });
    exe.addCSourceFile(.{ .file = .{ .path = "src/i18n.c" }, .flags = &.{} });
    exe.addCSourceFile(.{ .file = .{ .path = "src/lag.c" }, .flags = &.{} });
    exe.addCSourceFile(.{ .file = .{ .path = "src/main.c" }, .flags = &.{} });
    exe.addCSourceFile(.{ .file = .{ .path = "src/ood.c" }, .flags = &.{} });
    exe.addCSourceFile(.{ .file = .{ .path = "src/packet.c" }, .flags = &.{} });
    exe.addCSourceFile(.{ .file = .{ .path = "src/reset.c" }, .flags = &.{} });
    exe.addCSourceFile(.{ .file = .{ .path = "src/tamper.c" }, .flags = &.{} });
    exe.addCSourceFile(.{ .file = .{ .path = "src/throttle.c" }, .flags = &.{} });
    exe.addCSourceFile(.{ .file = .{ .path = "src/utils.c" }, .flags = &.{} });

    if (arch == .x86)
        exe.addCSourceFile(.{ .file = .{ .path = "etc/chkstk.s" }, .flags = &.{} });

    // Zig 0.11.0: addIncludeDir 更名为 addIncludePath
    exe.addIncludePath(.{ .path = b.fmt("external/{s}/include", .{windivert_dir}) });

    const iupLib = switch (arch) {
        .x64 => "external/iup-3.30_Win64_mingw6_lib",
        .x86 => "external/iup-3.30_Win32_mingw6_lib",
    };

    exe.addIncludePath(.{ .path = std.fs.path.join(b.allocator, &[2][]const u8{iupLib, "include"}) catch unreachable });
    exe.addCSourceFile(.{ .file = .{ .path = std.fs.path.join(b.allocator, &[2][]const u8{iupLib, "libiup.a"}) catch unreachable }, .flags = &.{} });

    exe.linkLibC();
    
    // Zig 0.11.0: addLibPath 更名为 addLibraryPath
    exe.addLibraryPath(.{ .path = b.fmt("external/{s}/{s}", .{windivert_dir, arch_tag}) });
    exe.linkSystemLibrary("WinDivert");
    exe.linkSystemLibrary("comctl32");
    exe.linkSystemLibrary("Winmm");
    exe.linkSystemLibrary("ws2_32");
    exe.linkSystemLibrary("kernel32");
    exe.linkSystemLibrary("gdi32");
    exe.linkSystemLibrary("comdlg32");
    exe.linkSystemLibrary("uuid");
    exe.linkSystemLibrary("ole32");

    const exe_install_step = b.addInstallArtifact(exe, .{});  
    if (conf == .Ship)
    {
        const remove_pdb_step = RemoveOutFile.create(b, "clumsy.pdb", prefix);
        remove_pdb_step.step.dependOn(&exe_install_step.step);
        b.getInstallStep().dependOn(&remove_pdb_step.step);
    }
    else
    {
        b.getInstallStep().dependOn(&exe_install_step.step);
    }

    const clean_all = b.step("clean", "purge zig-cache and zig-out");
    clean_all.dependOn(&b.addRemoveDirTree(b.install_path).step);
    //  TODO can't clean cache atm since build.exe is in it
    // clean_all.dependOn(&b.addRemoveDirTree("zig-cache").step);
}

pub const RemoveOutFile = struct {
    step: Step,
    builder: *Builder,
    rel_path: []const u8,
    prefix: []const u8,

    pub fn create(builder: *Builder, rel_path: []const u8, prefix: []const u8) *@This() {
        const self = builder.allocator.create(@This()) catch unreachable;
        self.* = . {
            .step = Step.init(.custom, builder.fmt("RemoveOutFile {s}", .{rel_path}), builder.allocator, make),
            .builder = builder,
            .rel_path = rel_path,
            .prefix = prefix,
        };
        return self;
    }

    fn make(step: *Step) anyerror!void {
        const self = @fieldParentPtr(RemoveOutFile, "step", step);
        // 文件安装到 prefix 子目录中，需要包含 prefix 路径
        const out_path = std.fs.path.join(self.builder.allocator, &[3][]const u8{self.builder.install_path, self.prefix, self.rel_path}) catch unreachable;
        std.fs.cwd().deleteFile(out_path) catch |err| {
            if (err != error.FileNotFound) return err;
        };
    }
};
