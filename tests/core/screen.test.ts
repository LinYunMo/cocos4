import { screenAdapter } from 'pal/screen-adapter';
import { Screen } from '../../cocos/core/platform/screen';

const adapter = screenAdapter as unknown as {
    rawDevicePixelRatio?: number;
    maxDevicePixelRatio?: number | null;
    init: jest.Mock;
};

describe('Screen startup maximum DPR', () => {
    let initSpy: jest.SpyInstance;

    beforeEach(() => {
        // The shared test bootstrap has already initialized PAL. Override only
        // this suite's adapter boundary without resetting the running test engine.
        Object.defineProperty(adapter, 'rawDevicePixelRatio', { value: 3.25, writable: true, configurable: true });
        Object.defineProperty(adapter, 'maxDevicePixelRatio', { value: null, writable: true, configurable: true });
        initSpy = jest.spyOn(screenAdapter, 'init').mockImplementation(options => {
            adapter.maxDevicePixelRatio = options.maxDevicePixelRatio ?? null;
        });
    });

    afterEach(() => {
        initSpy.mockRestore();
        delete adapter.rawDevicePixelRatio;
        delete adapter.maxDevicePixelRatio;
    });

    test('omitted configuration does not inject a platform-wide default', () => {
        const screen = new Screen();
        expect(screen.maxDevicePixelRatio).toBeNull();
        screen.init();
        expect(adapter.init.mock.calls[0][0]).not.toHaveProperty('maxDevicePixelRatio');
    });

    test('startup configuration is pending until initialization and reset clears it', () => {
        const screen = new Screen();
        screen.setMaxDevicePixelRatio(3.25);
        expect(screen.maxDevicePixelRatio).toBe(3.25);
        expect(adapter.init).not.toHaveBeenCalled();
        expect(adapter.maxDevicePixelRatio).toBeNull();
        screen.resetMaxDevicePixelRatio();
        screen.init();
        expect(screen.maxDevicePixelRatio).toBeNull();
        expect(adapter.init.mock.calls[0][0]).not.toHaveProperty('maxDevicePixelRatio');
    });

    test.each([1, 2, 2.5, 3.25, 4])('passes the exact configured cap %s to PAL', maximum => {
        const screen = new Screen();
        screen.setMaxDevicePixelRatio(maximum);
        screen.init();
        expect(adapter.init.mock.calls[0][0].maxDevicePixelRatio).toBe(maximum);
        expect(screen.maxDevicePixelRatio).toBe(maximum);
    });

    test.each([0, -1, 0.5, NaN, Infinity, -Infinity, '3', null, undefined])('rejects %s without changing the pending cap', value => {
        const screen = new Screen();
        screen.setMaxDevicePixelRatio(2.5);
        expect(() => screen.setMaxDevicePixelRatio(value as number)).toThrow(RangeError);
        expect(screen.maxDevicePixelRatio).toBe(2.5);
        screen.resetMaxDevicePixelRatio();
        expect(screen.maxDevicePixelRatio).toBeNull();
    });

    test('locks configuration before entering PAL, including same-value sets and resets', () => {
        const screen = new Screen();
        screen.setMaxDevicePixelRatio(2);
        adapter.init.mockImplementation(options => {
            expect(() => screen.setMaxDevicePixelRatio(2)).toThrow(/before Screen.init/);
            expect(() => screen.resetMaxDevicePixelRatio()).toThrow(/before Screen.init/);
            adapter.maxDevicePixelRatio = options.maxDevicePixelRatio;
        });
        screen.init();
        expect(() => screen.setMaxDevicePixelRatio(3)).toThrow(/before Screen.init/);
        expect(screen.maxDevicePixelRatio).toBe(2);
    });

    test('explicit configuration rejects old PAL before adapter initialization', () => {
        adapter.rawDevicePixelRatio = undefined;
        adapter.maxDevicePixelRatio = undefined;
        const screen = new Screen();
        screen.setMaxDevicePixelRatio(3);
        expect(() => screen.init()).toThrow(/installed PAL does not support/);
        expect(adapter.init).not.toHaveBeenCalled();
        expect(() => screen.resetMaxDevicePixelRatio()).toThrow(/before Screen.init/);
    });

    test('unconfigured startup retains compatibility with old PAL', () => {
        adapter.rawDevicePixelRatio = undefined;
        adapter.maxDevicePixelRatio = undefined;
        expect(() => new Screen().init()).not.toThrow();
    });

    test('rejects PAL that advertises properties but ignores the option', () => {
        adapter.init.mockImplementation(() => {});
        const screen = new Screen();
        screen.setMaxDevicePixelRatio(3);
        expect(() => screen.init()).toThrow(/PAL did not apply/);
    });
});
