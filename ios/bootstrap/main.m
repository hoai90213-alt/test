#import <UIKit/UIKit.h>
#include <dlfcn.h>

static NSString *ZDProbeRuntimeLibraries(void) {
  NSArray<NSString *> *libraryNames = @[
    @"libbox64.dylib",
    @"libzomdroid.dylib",
    @"libzomdroidlinker.dylib",
  ];

  NSString *frameworksPath = [NSBundle.mainBundle.bundlePath stringByAppendingPathComponent:@"Frameworks"];
  NSMutableArray<NSString *> *statusLines = [NSMutableArray array];

  for (NSString *name in libraryNames) {
    NSString *fullPath = [frameworksPath stringByAppendingPathComponent:name];
    if (![[NSFileManager defaultManager] fileExistsAtPath:fullPath]) {
      [statusLines addObject:[NSString stringWithFormat:@"%@: missing", name]];
      continue;
    }

    void *handle = dlopen(fullPath.UTF8String, RTLD_NOW | RTLD_GLOBAL);
    if (handle != NULL) {
      [statusLines addObject:[NSString stringWithFormat:@"%@: loaded", name]];
    } else {
      const char *err = dlerror();
      NSString *errText = err ? @(err) : @"unknown error";
      [statusLines addObject:[NSString stringWithFormat:@"%@: failed (%@)", name, errText]];
    }
  }

  return [statusLines componentsJoinedByString:@"\n"];
}

@interface ZDAppDelegate : UIResponder <UIApplicationDelegate>
@property(nonatomic, strong) UIWindow *window;
@end

@implementation ZDAppDelegate

- (BOOL)application:(UIApplication *)application
    didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
  (void)application;
  (void)launchOptions;

  self.window = [[UIWindow alloc] initWithFrame:[UIScreen mainScreen].bounds];

  UIViewController *rootViewController = [UIViewController new];
  rootViewController.view.backgroundColor = [UIColor systemBackgroundColor];

  UILabel *titleLabel = [UILabel new];
  titleLabel.translatesAutoresizingMaskIntoConstraints = NO;
  titleLabel.numberOfLines = 0;
  titleLabel.textAlignment = NSTextAlignmentCenter;
  titleLabel.font = [UIFont systemFontOfSize:24 weight:UIFontWeightSemibold];
  titleLabel.text = @"Zomdroid iOS Main Menu PoC\nBootstrap Build";

  UILabel *subtitleLabel = [UILabel new];
  subtitleLabel.translatesAutoresizingMaskIntoConstraints = NO;
  subtitleLabel.numberOfLines = 0;
  subtitleLabel.textAlignment = NSTextAlignmentCenter;
  subtitleLabel.font = [UIFont systemFontOfSize:14 weight:UIFontWeightRegular];
  subtitleLabel.textColor = [UIColor secondaryLabelColor];
  NSString *runtimeStatus = ZDProbeRuntimeLibraries();
  subtitleLabel.text = [NSString stringWithFormat:
      @"Build artifact is valid for TrollStore packaging.\nRuntime probe:\n%@",
      runtimeStatus];

  [rootViewController.view addSubview:titleLabel];
  [rootViewController.view addSubview:subtitleLabel];

  UILayoutGuide *guide = rootViewController.view.safeAreaLayoutGuide;
  [NSLayoutConstraint activateConstraints:@[
    [titleLabel.centerXAnchor constraintEqualToAnchor:guide.centerXAnchor],
    [titleLabel.centerYAnchor constraintEqualToAnchor:guide.centerYAnchor constant:-20.0],
    [titleLabel.leadingAnchor constraintGreaterThanOrEqualToAnchor:guide.leadingAnchor constant:24.0],
    [titleLabel.trailingAnchor constraintLessThanOrEqualToAnchor:guide.trailingAnchor constant:-24.0],
    [subtitleLabel.topAnchor constraintEqualToAnchor:titleLabel.bottomAnchor constant:16.0],
    [subtitleLabel.centerXAnchor constraintEqualToAnchor:guide.centerXAnchor],
    [subtitleLabel.leadingAnchor constraintGreaterThanOrEqualToAnchor:guide.leadingAnchor constant:24.0],
    [subtitleLabel.trailingAnchor constraintLessThanOrEqualToAnchor:guide.trailingAnchor constant:-24.0]
  ]];

  self.window.rootViewController = rootViewController;
  [self.window makeKeyAndVisible];
  return YES;
}

@end

int main(int argc, char *argv[]) {
  @autoreleasepool {
    return UIApplicationMain(argc, argv, nil, NSStringFromClass([ZDAppDelegate class]));
  }
}
